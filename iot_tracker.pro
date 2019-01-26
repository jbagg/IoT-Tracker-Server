QT+= core network
QT-= gui
TARGET= iot-trackerd
HEADERS= client.h   rmserver.h   rmclient.h   sslserver.h   sslserverworker.h   record.h global.h
SOURCES= client.cpp rmserver.cpp rmclient.cpp sslserver.cpp sslserverworker.cpp main.cpp
LIBS+= -lQtZeroConf
CONFIG+= c++11
    
                 
 

