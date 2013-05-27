# ## eqmake4 was here ###
CONFIG += designer \
    plugin \
    release
TARGET = $$qtLibraryTarget(customplaintexteditplugin)
TEMPLATE = lib
HEADERS = customplaintexteditplugin.h
SOURCES = customplaintexteditplugin.cpp
RESOURCES = 
LIBS += -L.
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
include(customplaintextedit.pri)
CONFIG -= debug_and_release \
    debug
CONFIG += release

DESTDIR = ../../Metalink_Common-libs
