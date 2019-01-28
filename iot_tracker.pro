QT+= core network
QT-= gui
TARGET= iot-trackerd
HEADERS= rmserver.h   rmclient.h   iotserver.h   iotserverworker.h   iotclient.h   record.h global.h
SOURCES= rmserver.cpp rmclient.cpp iotserver.cpp iotserverworker.cpp iotclient.cpp main.cpp
LIBS+= -lQtZeroConf
CONFIG+= c++11
    
                 
 

