QT+= core network gui widgets
TARGET= iottm
HEADERS= mainwindow.h   discover.h   link.h   dualboxlayout.h
SOURCES= mainwindow.cpp discover.cpp link.cpp dualboxlayout.cpp main.cpp
LIBS+= -lQtZeroConf
CONFIG+= c++11
    
                 
 

