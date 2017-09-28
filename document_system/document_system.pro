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
    csvdocumentwriter.cpp \
    csvpage.cpp

INCLUDEPATH += ../include/ \
    ../include/document \
    .

HEADERS += document.h \
    ../include/document/axittype.h \
    ../include/document/documentcreator.h \
    csvdocumentreader.h \
    csvdocumentwriter.h \
    ../include/document/page.h \
    csvpage.h \
    ../include/document/documentreader.h \
    ../include/document/documentwriter.h

OBJECTS_DIR = $${OUT_PWD}/../temp/obj
MOC_DIR =     $${OUT_PWD}/../temp/moc
DESTDIR =     $${OUT_PWD}/../lib

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    QT += axcontainer

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lExcelReader
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lExcelReader

    DEPENDPATH += $$PWD/../exel_reader

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libExelReader.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libExelReader.a
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/ExelReader.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/ExelReader.lib
}
