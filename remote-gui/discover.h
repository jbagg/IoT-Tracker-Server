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
   File name    : discovery.h
   Created      : 14 July 2015
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   zeroconf server discovery management
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef DISCOVER_H_
#define DISCOVER_H_

#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QHostAddress>
#include "mainwindow.h"
#include "link.h"
#include <QtZeroConf/qzeroconf.h>

class mainWindow;

class Discover : public QObject
{
	Q_OBJECT

public:
	Discover(Link &link, mainWindow &win);
	void start(void);

private:
	QMessageBox msgbox;
	mainWindow &window;
	Link &link;
	QZeroConf discover;
	QTimer timeout;


signals:

private slots:
	void discoveryTimedOut(void);
	void error(QZeroConf::error_t);
	void startLogin(QZeroConfService zcs);
	
protected:


};

#endif /* DISCOVER_H_ */
