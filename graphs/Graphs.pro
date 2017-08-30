
TEMPLATE = app
TARGET = Graphs
DESTDIR = ./bin
QT += core svg axcontainer widgets gui printsupport concurrent
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
    View/zoomer.h

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
    main.cpp

INCLUDEPATH += $$PWD/../include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rd_party/qwt/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rd_party/qwt/lib/ -lqwtd
else:unix:!macx: LIBS += -L$$OUT_PWD/../3rd_party/qwt/lib/ -lqwt

INCLUDEPATH += $$PWD/../3rd_party/qwt/src
DEPENDPATH += $$PWD/../3rd_party/qwt/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_party/qwt/lib/libqwt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_party/qwt/lib/libqwtd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_party/qwt/lib/qwt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_party/qwt/lib/qwtd.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../3rd_party/qwt/src/libqwt.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../exel_reader/ -lExelReader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../exel_reader/ -lExelReader

DEPENDPATH += $$PWD/../exel_reader

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../exel_reader/libExelReader.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../exel_reader/libExelReader.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../exel_reader/ExelReader.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../exel_reader/ExelReader.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../math_system/ -lMathSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../math_system/ -lMathSystem
else:unix:!macx: LIBS += -L$$OUT_PWD/../math_system/ -lMathSystem

DEPENDPATH += $$PWD/../math_system

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../math_system/libMathSystem.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../math_system/libMathSystem.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../math_system/MathSystem.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../math_system/MathSystem.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../math_system/libMathSystem.a
