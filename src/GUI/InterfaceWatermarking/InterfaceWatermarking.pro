#-------------------------------------------------
#
# Project created by QtCreator 2013-12-08T17:32:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InterfaceWatermarking
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    libwrapper.cpp

HEADERS  += mainwindow.h \
    libwrapper.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons/qstyle.qrc

INCLUDEPATH += $$PWD/../../libwatermark
DEPENDPATH += $$PWD/../../libwatermark

LIBS+=-lsndfile  -lfftw3
