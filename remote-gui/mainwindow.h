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
   File name    : mainWindow.h
   Created      : 05 June 2015
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Main Window
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
#include "dualboxlayout.h"
#include "link.h"
#include "discover.h"

class Discover;

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow();

public slots:
	void updateCps(const QString &cps);
	void updateTotalDevices(const QString &cps) { totalDevicesLabel.setText(QString("Total Devices: %1").arg(cps.trimmed())); }
	void update(const QByteArray &data);

private:
	void buildGui(void);
	void closeEvent(QCloseEvent *event);
	Link link;
	QMessageBox errorMsgbox;
	Discover *discover;

	// GUI
	QVBoxLayout mainLayout;
	DualBoxLayout reportLayout;
	QVBoxLayout modelsLayout;
	QVBoxLayout versionsLayout;
	QLabel cpsLabel;
	QLabel totalDevicesLabel;
	QLabel modelsLabel;
	QLabel versionsLabel;
	QPlainTextEdit versionArea;
	QPlainTextEdit modelArea;

private slots:
	void srvDisconnect(void);

};

#endif /* MAINWINDOW_H_ */
