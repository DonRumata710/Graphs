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

SOURCES += \
    documentcreator.cpp \
    csvdocumentreader.cpp \
    csvdocumentwriter.cpp

INCLUDEPATH += ../include/ \
    ../include/document \
    .

HEADERS += document.h \
    ../include/document/documentwriter.h \
    ../include/document/axittype.h \
    ../include/document/documentreader.h \
    ../include/document/documentcreator.h \
    csvdocumentreader.h \
    csvdocumentwriter.h

DESTDIR = $${OUT_PWD}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
