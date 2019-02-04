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
   File name    : link.cpp
   Created      : 08 June 2015
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Handles communication to the server
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <QSslKey>
#include <QSslCertificate>
#include <random>
#include "link.h"


Link::Link()
{
	connect(&server, SIGNAL(disconnected()), this, SLOT(srvDisconnect()));

	#ifdef IOT_SERVER_NEEDS_IOT_AUTHENTICATION
	server.setPrivateKey("iot_blue_local.key");
	server.setLocalCertificate("iot_blue_local.pem");
	if (server.localCertificate().isNull())
		qDebug("No certificate for authentication");
	if (server.privateKey().isNull())
		qDebug("No private key for authentication");
	#endif
}

void Link::srvConnect()
{
	QString version, model;
	
	#ifdef ENCRYPTED_LINK
	server.setPeerVerifyMode(QSslSocket::VerifyNone);
	server.connectToHostEncrypted("127.0.0.1", 12345);
	if (server.waitForEncrypted(5000)) {
	#else
	server.connectToHost("127.0.0.1", 12345);
	if (server.waitForConnected(5000)) {
	#endif
		std::random_device rd;
		std::mt19937_64 eng(rd());
		std::uniform_int_distribution<size_t> distr;
		size_t x = (size_t)distr(eng);
		x&= 0x1FFFF;

		if (x & 1) {
			model = "Model A";
			version = "2.4";
		}
		else if (x&2) {
			model = "Model T";
			version = "1.0";
		}
		else if (x&4) {
			model = "Model 3";
			version = "2.1";
		}
		else {
			model = "Model F";
			version = "1.5";
		}
		
		server.write(QString("id=%1\nmodel=%2\nversion=%3\nboot\n").arg(x).arg(model).arg(version).toUtf8());
		server.waitForBytesWritten(-1);
		server.close();
	}
	else
	{
		qDebug("Unable to connect");
	}
}

void Link::srvDisconnect(void)
{
	//qDebug("disconnect");
	//srvConnect();
	emit disconnected();
}
