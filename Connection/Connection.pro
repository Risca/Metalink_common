QT       += network

TARGET = Connection
TEMPLATE = lib

DEFINES += CONNECTION_LIBRARY

SOURCES += connection.cpp

HEADERS += connection.h\
        Connection_global.h

INCLUDEPATH += ../ChatCommand
LIBS += -L../../Metalink_Common-libs -lChatCommand

DESTDIR = ../../Metalink_Common-libs
