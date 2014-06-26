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
    TraderSpi.cpp

HEADERS  += mainwindow.h \
    TraderSpi.h

FORMS    += mainwindow.ui
