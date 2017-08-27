#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T11:19:22
#
#-------------------------------------------------

QT       -= core gui

TARGET = document_system
TEMPLATE = lib

DEFINES += DOCUMENT_SYSTEM_LIBRARY

SOURCES += document.cpp \
    csvdocument.cpp

HEADERS += document.h \
    csvdocument.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
