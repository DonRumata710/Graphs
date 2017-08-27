#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T13:56:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = MathSystem
TEMPLATE = lib

CONFIG += staticlib

DEFINES += MATH_SYSTEM_LIBRARY

SOURCES += \
    graphmodel.cpp \
    plotdata.cpp \
    threadscontrol.cpp \
    wavelemodel.cpp \
    waveletdata.cpp \
    waveletfunction.cpp \
    waveletproducer.cpp \
    row.cpp

INCLUDEPATH += \
    $${PWD}/../include/ \
    $${PWD}/../include/math

HEADERS += \
    abstractmodel.h \
    graphmodel.h \
    plotdata.h \
    row.h \
    threadscontrol.h \
    wavelemodel.h \
    waveletdata.h \
    waveletfunction.h \
    waveletproducer.h \
    waveletstep.h

DESTDIR = $${OUT_PWD}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
