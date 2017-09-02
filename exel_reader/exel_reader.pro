#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T17:33:26
#
#-------------------------------------------------

QT       -= gui

TARGET = ExelReader
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++14

DEFINES += EXELREADER_LIBRARY

SOURCES += \
    excelfile.cpp \
    exceldocumentwriter.cpp \
    exceldocumentreader.cpp

INCLUDEPATH += ../include/ \
    ../include/excel/

HEADERS += \
    excelfile.h \
    ../include/excel/exceldocumentreader.h \
    ../include/excel/exceldocumentwriter.h

OBJECTS_DIR = $${OUT_PWD}/../temp/obj
MOC_DIR =     $${OUT_PWD}/../temp/moc
DESTDIR =     $${OUT_PWD}/../lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../document_system/ -lDocumentSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../document_system/ -lDocumentSystem

INCLUDEPATH += $$PWD/../document_system
DEPENDPATH += $$PWD/../document_system
