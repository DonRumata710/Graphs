#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T13:56:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = MathSystem
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

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
    row.h \
    threadscontrol.h \
    wavelemodel.h \
    waveletdata.h \
    waveletfunction.h \
    waveletproducer.h \
    ../include/math/abstractmodel.h \
    ../include/math/graphmodel.h \
    ../include/math/row.h \
    ../include/math/wavelemodel.h \
    ../include/math/waveletdata.h \
    intervals.h \
    ../include/math/waveletproducer.h \
    ../include/math/waveletstep.h \
    ../include/math/intervals.h \
    ../include/math/plotdata.h

OBJECTS_DIR = $${OUT_PWD}/../temp/obj
MOC_DIR =     $${OUT_PWD}/../temp/moc
DESTDIR =     $${OUT_PWD}/../lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CXXFLAGS+= -openmp
QMAKE_LFLAGS +=  -openmp
