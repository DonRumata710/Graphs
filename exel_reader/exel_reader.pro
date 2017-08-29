#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T17:33:26
#
#-------------------------------------------------

QT       -= gui

TARGET = ExelReader
TEMPLATE = lib

CONFIG += staticlib

DEFINES += EXELREADER_LIBRARY

SOURCES += \
    exceldocument.cpp \
    excelfile.cpp

INCLUDEPATH += ../include/ \
    ../include/excel/

HEADERS += \
    ../include/excel/exceldocument.h \
    excelfile.h

DESTDIR = $${OUT_PWD}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../document_system/ -lDocumentSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../document_system/ -lDocumentSystem

INCLUDEPATH += $$PWD/../document_system
DEPENDPATH += $$PWD/../document_system
