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
   File name    : mainWindow.cpp
   Created      : 05 June 2015
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
   Main Window
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDate>
#include <QCryptographicHash>
#include <QDesktopWidget>
#include "mainwindow.h"

mainWindow::mainWindow()
{
	discover = new Discover(link, *this);
	buildGui();

	connect(&link, &Link::disconnected, this, &mainWindow::srvDisconnect);
	connect(&link, &Link::updateCps, this, &mainWindow::updateCps);
	connect(&link, &Link::update, this, &mainWindow::update);

	discover->start();
}

void mainWindow::buildGui(void)
{
	mainLayout.addWidget(&cpsLabel);
	mainLayout.addLayout(&reportLayout);
	reportLayout.addWidget(&modelArea);
	reportLayout.addWidget(&versionArea);

	reportLayout.setLayoutDirections(QBoxLayout::LeftToRight, QBoxLayout::TopToBottom);

	cpsLabel.setText("CPS: ?");
	modelArea.setReadOnly(1);
	versionArea.setReadOnly(1);

	errorMsgbox.setIcon(QMessageBox::Warning);

	QWidget *widget = new QWidget;
	widget->setLayout(&mainLayout);
	setCentralWidget(widget);
	show();
	setWindowState(Qt::WindowMaximized);
}

void mainWindow::srvDisconnect(void)
{
	QMessageBox msgbox;
	msgbox.setIcon(QMessageBox::Critical);
	msgbox.setText(tr("Server Disconnected - exiting"));
	msgbox.setStandardButtons(QMessageBox::Ok);
	msgbox.exec();
	exit(1);
}

void mainWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	exit(1);
}

void mainWindow::updateCps(const QString &cps)
{
	cpsLabel.setText(QString("CPS: %1").arg(cps.trimmed()));
}

void mainWindow::update(QByteArray const &jsonModelData)
{
	//modelArea.setPlainText("This Cow is cool\nBob is a Dog");
	//versionArea.setPlainText("This Cow is cool\nBob is a Dog");
}
