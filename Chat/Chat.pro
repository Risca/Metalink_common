TARGET = Chat
TEMPLATE = lib

DEFINES += CHAT_LIBRARY

HEADERS += \
    chat.h \
    Chat_global.h

SOURCES += \
    chat.cpp

DEPENDPATH += ../ChatCommand
INCLUDEPATH += ../ChatCommand
LIBS += -L../../Metalink_Common-libs -lChatCommand

DESTDIR = ../../Metalink_Common-libs
