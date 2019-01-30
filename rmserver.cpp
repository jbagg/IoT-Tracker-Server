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
   File name    : rmserver.cpp
   Created      : 12 March 2018
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Simple secure TCP socket server for the remote monitors to connect to.
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <QSslSocket>
#include "rmserver.h"

RemoteMonitorServer::RemoteMonitorServer(QObject *parent) : QTcpServer(parent)
{
	QFile keyFile("red_local.key");
	keyFile.open(QIODevice::ReadOnly);
	key = QSslKey(keyFile.readAll(), QSsl::Rsa);
	keyFile.close();

	QFile certFile("red_local.pem");
	certFile.open(QIODevice::ReadOnly);
	cert = QSslCertificate(certFile.readAll());
	certFile.close();

	if (!listen(QHostAddress::Any, RM_PORT)) {
		qCritical() << "Unable to start the TCP server";
		exit(1);
	}

	zeroConf.startServicePublish("IoT-Tracker", "_iot_tracker._tcp", "", RM_PORT);
}

void RemoteMonitorServer::incomingConnection(qintptr socketDescriptor)
{
	RemoteMonitorClient *client;
	QSslSocket *sslSocket = new QSslSocket(this);

	sslSocket->setSocketDescriptor(socketDescriptor);
	#ifdef ENABLE_RM_ENCRYPTION
	connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
	sslSocket->setPrivateKey(key);
	sslSocket->setLocalCertificate(cert);
	sslSocket->setCaCertificates(caCert);
	#ifdef ENABLE_RM_AUTHENTICATION
	sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);
	#else
	sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
	#endif
	sslSocket->startServerEncryption();
	#endif

	client = new RemoteMonitorClient(sslSocket, *this);
	clients.append(client);
}

void RemoteMonitorServer::sslErrors(const QList<QSslError> &errors)
{
	#ifdef ENABLE_RM_AUTHENTICATION
	foreach (const QSslError &error, errors)
		qDebug() << error.errorString();
	#else
	QSslSocket* sslSocket = qobject_cast<QSslSocket*>(sender());
	sslSocket->ignoreSslErrors(errors);
	#endif
}

void RemoteMonitorServer::updateClientsCPS(ssize_t cps, ssize_t totalDevices)
{
	QByteArray cpsValue = QString("cps=%1\ntotal_devices=%2\n").arg(cps).arg(totalDevices).toUtf8();

	foreach (RemoteMonitorClient *client, clients)
		client->write(cpsValue);
}

void RemoteMonitorServer::updateClients(QByteArray jsonDoc)
{
	foreach (RemoteMonitorClient *client, clients) {
		client->write(QString("update=%1\n").arg(jsonDoc.size(), 0, 10).toUtf8());
		client->write(jsonDoc);
	}
}
