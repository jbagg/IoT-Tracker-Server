/**************************************************************************************************
---------------------------------------------------------------------------------------------------
	Copyright (c) 2015-2019, Jonathan Bagg
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
   File name    : client.cpp
   Created      : 01 June 2015
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Object to handle connected clients.
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <QByteArray>
#include <QFile>
#include <QMutexLocker>
#include "client.h"
#include "sslserver.h"

Client::Client(SslServerWorker &parent, QSslSocket *newConnection, SslServer &server) : server(server), serverWorker(parent), socket(newConnection)
{
	connect(socket, &QSslSocket::readyRead, this, &Client::rx);
	connect(socket, &QSslSocket::disconnected, this, &Client::disconnect);
}

void Client::rx(void)
{
	while (socket->canReadLine()) {
		QString line = socket->readLine();	// fixme set read size limit?
		line.chop(1); // remove '\n'
		if (!line.size())
			continue;

		QString cmd = line.section('=', 0, 0);
		QString value = line.section('=', 1, 1);

		if (cmd == "id")
			id = value;
		else if (cmd == "version")
			version = value;
		else if (cmd == "model")
			model = value;
		else if (cmd == "boot")
			reportBoot();
		else if (cmd == "24hrs")
			report24hrs();
		else
			qDebug() << cmd;
	}

}

void Client::reportBoot()
{
	Record *record;
	size_t idn = static_cast<size_t>(id.toInt());

	QMutexLocker lock(&server.recordLocker);
	if (!server.records.contains(idn))
	{
		record = new Record;
		record->id = idn;
		record->checkIn = false;
		record->model = model;
		record->lastBoot = QDateTime::currentDateTime();
		server.records.insert(idn, record);
	}
	else
		record = server.records[idn];

	record->totalBoots++;
	record->version = version;
}

void Client::report24hrs()
{
	Record *record;
	size_t idn = static_cast<size_t>(id.toInt());

	QMutexLocker lock(&server.recordLocker);
	if (!server.records.contains(idn))
	{
		record = new Record;
		record->id = idn;
		record->model = model;
		server.records.insert(idn, record);
	}
	else
		record = server.records[idn];

	record->checkIn = true;
	record->version = version;
}

void Client::disconnect(void)
{
	serverWorker.removeClient(this);
	socket->deleteLater();
	delete this;	
}

void Client::sendFile(QString &fileName)
{
	QFile file("files/" + fileName);
	if (file.open(QIODevice::ReadOnly)) {
		socket->write(QString("file=%1\n").arg(file.size(), 0, 10).toUtf8());
		socket->write(file.readAll());
	}
	else
		socket->write("no_file\n");
	file.close();
}

void Client::sendManifest()
{
	QFile file("files/manifest");
	if (file.open(QIODevice::ReadOnly)) {
		socket->write(QString("manifest=%1\n").arg(file.size(), 0, 10).toUtf8());
		socket->write(file.readAll());
	}
	else
		socket->write("no_manifest\n");
	file.close();
}
