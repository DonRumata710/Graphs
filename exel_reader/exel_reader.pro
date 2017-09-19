#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T17:33:26
#
#-------------------------------------------------

QT -= gui
QT += core axcontainer

TARGET = ExelReader
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

DEFINES += EXELREADER_LIBRARY

SOURCES += \
    excelfile.cpp \
    exceldocumentwriter.cpp \
    exceldocumentreader.cpp \
    excelpage.cpp

INCLUDEPATH += ../include/ \
    ../include/excel/

HEADERS += \
    excelfile.h \
    ../include/excel/exceldocumentreader.h \
    ../include/excel/exceldocumentwriter.h \
    excelpage.h

OBJECTS_DIR = $${OUT_PWD}/../temp/obj
MOC_DIR =     $${OUT_PWD}/../temp/moc
DESTDIR =     $${OUT_PWD}/../lib

INCLUDEPATH += $$PWD/../document_system
