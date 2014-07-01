#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T16:00:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CTPTrader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TraderSpi.cpp \
    cmysql_api.cpp \
    chstimer.cpp \
    widget.cpp \
    help.cpp

HEADERS  += mainwindow.h \
    TraderSpi.h \
    cmysql_api.h \
    chstimer.h \
    widget.h \
    help.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/CTP/api/trade/linux64/lib/release/ -lthosttraderapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/CTP/api/trade/linux64/lib/debug/ -lthosttraderapi
else:unix: LIBS += -L$$PWD/CTP/api/trade/linux64/lib/ -lthosttraderapi

INCLUDEPATH += $$PWD/CTP/api/trade/linux64
DEPENDPATH += $$PWD/CTP/api/trade/linux64



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/ -lmysqlclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/ -lmysqlclient
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lmysqlclient

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/libmysqlclient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/libmysqlclient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/mysqlclient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/mysqlclient.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/libmysqlclient.a
