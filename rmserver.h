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
   File name    : rmserver.h
   Created      : 12 March 2018
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Simple secure TCP socket server for the remote monitors to connect to.
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef RMSERVER_H
#define RMSERVER_H

#include <QTcpServer>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslSocket>
#include <QtZeroConf/qzeroconf.h>
#include "global.h"
#include "rmclient.h"



class RemoteMonitorServer : public QTcpServer
{
	Q_OBJECT

public:
	RemoteMonitorServer(QObject *parent = nullptr);
	inline void removeClient(RemoteMonitorClient *client) {clients.removeAt(clients.indexOf(client));}
	void updateClientsCPS(ssize_t cps, ssize_t totalDevices);
	void updateClients(QByteArray jsonDoc);

private:
	QZeroConf zeroConf;
	QSslKey key;
	QSslCertificate cert;
	QList<QSslCertificate> caCert;
	QList <RemoteMonitorClient *> clients;

private slots:
	void sslErrors(const QList<QSslError> &errors);

protected:
	void incomingConnection(qintptr socketDescriptor);
};

#endif // RMSERVER_H
