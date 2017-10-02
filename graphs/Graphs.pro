
TEMPLATE = app
TARGET = Graphs

OBJECTS_DIR = $${OUT_PWD}/../temp/obj
MOC_DIR =     $${OUT_PWD}/../temp/moc
DESTDIR =     $${OUT_PWD}/../bin

CONFIG += c++14

QT += core svg widgets gui printsupport concurrent
DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB QT_SVG_LIB QT_PRINTSUPPORT_LIB QT_CONCURRENT_LIB
FORMS += \
    approximation.ui \
    correlation.ui \
    help.ui \
    main_win.ui \
    simple_wavelet.ui \
    smoothing.ui \
    spectr.ui \
    wavelet.ui

HEADERS += \
    Presenter/graphpresenter.h \
    Presenter/matrixrasterdata.h \
    Presenter/spectrogrampresenter.h \
    Presenter/tabpresenter.h \
    View/application.h \
    View/choiseexception.h \
    View/correlation.h \
    View/curve.h \
    View/grid.h \
    View/helper.h \
    View/logscale.h \
    View/multygraph.h \
    View/plotmanager.h \
    View/rangemaster.h \
    View/scaledraw.h \
    View/smoothing.h \
    View/spectr.h \
    View/swaveletperiod.h \
    View/wavelet.h \
    View/zoomer.h \
    Presenter/maththread.h

SOURCES += \
    Presenter/graphpresenter.cpp \
    Presenter/matrixrasterdata.cpp \
    Presenter/spectrogrampresenter.cpp \
    Presenter/tabpresenter.cpp \
    View/application.cpp \
    View/correlation.cpp \
    View/helper.cpp \
    View/logscale.cpp \
    View/multygraph.cpp \
    View/plotmanager.cpp \
    View/rangemaster.cpp \
    View/smoothing.cpp \
    View/spectr.cpp \
    View/swaveletperiod.cpp \
    View/wavelet.cpp \
    View/zoomer.cpp \
    main.cpp \
    Presenter/maththread.cpp

INCLUDEPATH += $$PWD/../include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lqwt
else:unix:!macx: LIBS += -L$$OUT_PWD/../lib/ -lqwt

INCLUDEPATH += $$PWD/../3rd_party/qwt/src
DEPENDPATH += $$PWD/../3rd_party/qwt/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libqwt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libqwt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/qwt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/qwt.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../lib/libqwt.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lMathSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lMathSystem
else:unix:!macx: LIBS += -L$$OUT_PWD/../lib/ -lMathSystem

DEPENDPATH += $$PWD/../math_system

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libMathSystem.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libMathSystem.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/MathSystem.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/MathSystem.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../lib/libMathSystem.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lDocumentSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lDocumentSystem
else:unix:!macx: LIBS += -L$$OUT_PWD/../lib/ -lDocumentSystem

DEPENDPATH += $$PWD/../document_system

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libDocumentSystem.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libDocumentSystem.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/DocumentSystem.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/DocumentSystem.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../lib/libDocumentSystem.a

win32 {
    QT += axcontainer

    CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lExelReader
    else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/ -lExelReader

    DEPENDPATH += $$PWD/../exel_reader

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libExelReader.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/libExelReader.a
    else:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/ExelReader.lib
    else:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../lib/ExelReader.lib
}

win32:!win32-g++: QMAKE_CXXFLAGS+= -openmp
else: QMAKE_CXXFLAGS+= -fopenmp

win32:!win32-g++: QMAKE_LFLAGS +=  -openmp
else: QMAKE_LFLAGS +=  -fopenmp
