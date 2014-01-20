#-------------------------------------------------
#
# Project created by QtCreator 2013-12-08T17:32:49
#
#-------------------------------------------------

QT     += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport



TARGET = InterfaceWatermarking
TEMPLATE = app
DESTDIR = $$PWD/../../../output
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    libwrapper.cpp \
    qcustomplot.cpp \
    codeeditor.cpp \
    settingsmanager.cpp

HEADERS  += mainwindow.h \
    libwrapper.h \
    qcustomplot.h \
    codeeditor.h \
    settingsmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons/qstyle.qrc

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

LIBS+=-lsndfile  -lfftw3
