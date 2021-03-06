/**************************************************************************************************
---------------------------------------------------------------------------------------------------
	Copyright (c) 2019, Jonathan Bagg
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
   File name    : main.cpp
   Created      : 22 Jan 2019
   Author(s)    : Jonathan Bagg
---------------------------------------------------------------------------------------------------
**************************************************************************************************/
#include <QDebug>
#include <QApplication>
#include <QStringList>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QStringList args = app.arguments();

	app.setApplicationName("IoT Tracker - Live Monitor");
	app.setApplicationVersion("0.7");

	if (args.size() == 2 && args.at(1) == "--version")
	{
		qDebug() << app.applicationName().toUtf8() << app.applicationVersion().toUtf8();
		return 0;
	}

	mainWindow window;

	return app.exec();
}
