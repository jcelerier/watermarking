TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
DESTDIR = $$PWD/../../output



INCLUDEPATH += $$PWD/../libwatermark
DEPENDPATH += $$PWD/../libwatermark

LIBS+=-lsndfile -lfftw3
