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
   File name    : link.h
   Created      : 08 June 2015
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Handles communication to the server
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef LINK_H_
#define LINK_H_

#include <QSslSocket>
#include <QTcpSocket>

//#define ENCRYPTED_LINK
//#define IOT_SERVER_NEEDS_REMOTE_MONITOR_AUTHENTICATION

class Link : public QObject
{
	Q_OBJECT

public:
	Link();
	bool srvConnect(const QString &host, size_t port);
	void tx(QString cmd);

private:
	#ifdef ENCRYPTED_LINK
	QSslSocket server;
	#else
	QTcpSocket server;
	#endif
	ssize_t binaryLength;
	bool binaryMode;

Q_SIGNALS:
	void disconnected(void);
	void updateCps(const QString &cps);
	void updateTotalDevices(const QString &cps);
	void update(QByteArray const &jsonData);

private slots:
	void rx(void);
	void srvDisconnect(void);
};

#endif /* LINK_H_ */
