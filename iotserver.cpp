/**************************************************************************************************
---------------------------------------------------------------------------------------------------
	Copyright (c) 2018-2019, Jonathan Bagg
	All rights reserved.

	 Redistribution and use in source and binary forms, with or without modification, are permitted
	 provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice, this list of
	  conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice, this list of
	  conditions and the following disclaimer in the documentation and/or other materials provided
	  with the distribution.
	* Neither the name of Jonathan Bagg nor the names of its contributors may be used to
	  endorse or promote products derived from this software without specific prior written permission.
	* This software may not be used to track personal or identifying information.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------------------------------
   Project name : IoT Tracker Server
   File name    : iotserver.cpp
   Created      : 12 March 2018
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Simple secure TCP socket server
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <QSslSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "iotserver.h"

IoTServer::IoTServer(QObject *parent) : QTcpServer(parent)
{
	dispatchId = 0;
	countMetricsStart = true;
	qRegisterMetaType<qintptr>("qintptr");

	QFile keyFile("red_local.key");
	keyFile.open(QIODevice::ReadOnly);
	key = QSslKey(keyFile.readAll(), QSsl::Rsa);
	keyFile.close();

	QFile certFile("red_local.pem");
	certFile.open(QIODevice::ReadOnly);
	cert = QSslCertificate(certFile.readAll());
	certFile.close();

	size_t i;
	for (i=0; i<THREADS; i++)
	{
		workers[i] = new IoTServerWorker(*this);
		workers[i]->moveToThread(&threads[i]);
		threads[i].start();
	}

	if (!listen(QHostAddress::Any, IOT_PORT)) {
		qCritical() << "Unable to start the TCP server";
		exit(1);
	}

	rmServer = new RemoteMonitorServer();

	connect(&oneSec, &QTimer::timeout, this, &IoTServer::measure);
	connect(&periodicWorkTimer, &QTimer::timeout, this, &IoTServer::countMetrics);
	oneSec.start(1000);
	periodicWorkTimer.start(PERIODIC_WORK_TICK_TIME);
}

void IoTServer::measure()
{
	qDebug() << serves << records.size();
	rmServer->updateClientsCPS(serves);
	serves = 0;
}

void IoTServer::incomingConnection(qintptr socketDescriptor)
{
	serves++;
	QMetaObject::invokeMethod(workers[dispatchId], "newConnection", Q_ARG(qintptr, socketDescriptor));
	dispatchId++;
	if (dispatchId >= THREADS)
		dispatchId = 0;
}

void IoTServer::countMetrics()
{
	size_t i;

	if (records.isEmpty())
		return;

	if (!recordLocker.tryLock(0))  // use tryLock() to keep this thread free to handle incoming connections.
		return;
	if (countMetricsStart) {
		countMetricsStart = false;
		models.clear();
		versions.clear();
		rec = recordList.begin();
	}

	i=0;
	// only do a small number of records per periodic work tick so this thread is free to handle incoming connections.
	while (i<RECORDS_PER_PERIODIC_TICK && rec != recordList.end()) {
		// count models
		if (models.contains((*rec)->model))
			models[(*rec)->model]++;
		else
			models.insert((*rec)->model, 1);

		//count versions
		if (versions.contains((*rec)->version))
			versions[(*rec)->version]++;
		else
			versions.insert((*rec)->version, 1);

		i++;
		rec++;
	}

	if (rec == recordList.end())
		countMetricsStart = true;

	recordLocker.unlock();

	if (countMetricsStart)
		buildJsonDoc();
}

void IoTServer::buildJsonDoc()
{
	QMap<QString, size_t>::iterator i;
	QJsonObject jObject, jModels, jVersions;

	for (i = models.begin(); i != models.end(); i++)
		jModels.insert(i.key(), QJsonValue::fromVariant(QString("%1").arg(i.value())));

	for (i = versions.begin(); i != versions.end(); i++)
		jVersions.insert(i.key(), QJsonValue::fromVariant(QString("%1").arg(i.value(), 0, 10)));

	jObject.insert("models", jModels);
	jObject.insert("versions", jVersions);

	QJsonDocument jDoc(jObject);
	rmServer->updateClients(jDoc.toJson());
}

