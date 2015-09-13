#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T13:35:07
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RemoteViewer
TEMPLATE = app

SOURCES += main.cpp\
        remoteviewer.cpp \
    newvestdialog.cpp \
    ekgconnection.cpp \
    discovervestdialog.cpp \
    vestfinder.cpp \
    qcustomplot.cpp \
    iirfilter.cpp \
    settingsdialog.cpp

HEADERS  += remoteviewer.h \
    newvestdialog.h \
    ekgconnection.h \
    discovervestdialog.h \
    vestfinder.h \
    qcustomplot.h \
    iirfilter.h \
    settingsdialog.h

FORMS    += remoteviewer.ui


LIBS        += -lvlc-qt -lvlc-qt-widgets

# Edit below for custom library location
#Desktop and Laptop Path
LIBS     += -LE:\Qt\libvlc-qt\lib -lvlc-qt -lvlc-qt-widgets
INCLUDEPATH += E:\Qt\libvlc-qt\include
