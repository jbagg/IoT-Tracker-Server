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
   File name    : iotserver.h
   Created      : 12 March 2018
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Simple secure TCP socket server
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef IOTSERVER_H
#define IOTSERVER_H

#include <QTcpServer>
#include <QTimer>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslSocket>
#include <QThread>
#include <QMutex>
#include <QLinkedList>
#include "global.h"
#include "rmserver.h"
#include "iotclient.h"
#include "iotserverworker.h"
#include "record.h"

class IoTServerThread;

class IoTServer : public QTcpServer
{
	Q_OBJECT

public:
	IoTServer(QObject *parent = nullptr);
	QSslKey key;
	QSslCertificate cert;
	QList<QSslCertificate> caCert;
	// records (QHash) is used for finding and updating records.  Lookup and insertion time is critical.
	//   Lookup is constant amortized time "Amort. O(1)".  Worst case is O(n).
	//   Insertion time is constant amortized time "Amort. O(1)".  Worst case is O(n).
	QHash <size_t, Record*> records;
	// recordList is used to build reports for the remote monitors.  No lookup are done, list read from begining to end.  Only insertion time is critical.
	//   Insertion time is constatnt time O(1).
	QLinkedList <Record*> recordList;
	QMutex recordLocker;

private:
	QThread threads[THREADS];
	IoTServerWorker *workers[THREADS];
	RemoteMonitorServer *rmServer;
	size_t dispatchId;
	QTimer oneSec;
	QTimer periodicWorkTimer;
	int32_t serves;

	// Metrics compilation
	void buildJsonDoc();
	QMap <QString, size_t> versions;
	QMap <QString, size_t> models;
	QLinkedList <Record*>::iterator rec;
	bool countMetricsStart;

private slots:
	void measure();
	void countMetrics();

protected:
	void incomingConnection(qintptr socketDescriptor);
};

#endif // IOTSERVER_H
