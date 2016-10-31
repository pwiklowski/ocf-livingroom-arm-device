QT += core serialport
QT -= gui

LIBS += -L../liboic -loic
LIBS += -L../libcoap -lcoap


INCLUDEPATH += ../liboic
INCLUDEPATH += ../std
INCLUDEPATH += ../libcoap

CONFIG += c++11

TARGET = ocf-livingroom-arm-device
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    application.cpp

HEADERS += \
    application.h

