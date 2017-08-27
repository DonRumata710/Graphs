#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T11:19:22
#
#-------------------------------------------------

QT       -= core gui

TARGET = DocumentSystem
TEMPLATE = lib

CONFIG += staticlib

DEFINES += DOCUMENT_SYSTEM_LIBRARY

SOURCES += csvdocument.cpp

INCLUDEPATH += ../include/ \
    ../include/document \
    .

HEADERS += document.h \
    csvdocument.h \
    ../include/document/document.h

DESTDIR = $${OUT_PWD}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
