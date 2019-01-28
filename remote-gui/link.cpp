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
#include "link.h"

Link::Link()
{
	connect(&server, SIGNAL(readyRead()), this, SLOT(rx()));
	connect(&server, SIGNAL(disconnected()), this, SLOT(srvDisconnect()));

	// https://dst.lbl.gov/~boverhof/openssl_certs.html        // add -days 31000
	// this client certificate is requested by the server and validated against the CA certificate in the server
	QByteArray keyArray("");
	server.setPrivateKey(QSslKey(keyArray, QSsl::Rsa));

	QByteArray certArray("");
	server.setLocalCertificate(QSslCertificate(certArray));
}

bool Link::srvConnect(const QString &host, size_t port)
{
	server.setPeerVerifyMode(QSslSocket::VerifyNone);
	server.connectToHostEncrypted(host, port);
	//server.connectToHost("127.0.0.1", port);
	if (server.waitForEncrypted(5000)) {
		//fixme - do somthing
		return 1;
	}
	else
		return 0;
}

void Link::tx(QString cmd)
{
	server.write(cmd.toUtf8());
}

void Link::rx(void)
{
	while ((!binaryMode && server.canReadLine()) || (binaryMode && server.bytesAvailable() >= binaryLength)) {
		if (binaryMode) {
			emit update(server.read(binaryLength));
			binaryMode = false;
		}
		else {
			QString line = server.readLine();	// fixme set read size limit?
			line.chop(1); // remove '\n'
			if (!line.size())
				continue;

			QString cmd = line.section('=', 0, 0);
			QString value = line.section('=', 1, 1);

			if (cmd == "cps") {
				emit updateCps(value);
			}
			else if (cmd == "update") {
				binaryLength = value.toInt();	// fixme set binary size limit?
				binaryMode = true;
			}
			else
				qDebug() << cmd << cmd.size();
		}
	}
}

void Link::srvDisconnect(void)
{
	emit disconnected();
}
