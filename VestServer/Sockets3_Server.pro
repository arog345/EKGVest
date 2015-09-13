QT       += core

QT       -= gui

QT       += network

TARGET = Sockets3_Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    simplestreamer.cpp

HEADERS += \
    simplestreamer.h

LIBS += -L/usr/local/include -lbcm2835
