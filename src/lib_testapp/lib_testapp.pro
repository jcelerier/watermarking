TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
DESTDIR = $$PWD/../../output


unix|win32: LIBS += -L$$PWD/../../output/ -lwaterkmark

INCLUDEPATH += $$PWD/../libwatermark
DEPENDPATH += $$PWD/../libwatermark

win32: PRE_TARGETDEPS += $$PWD/../../output/waterkmark.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../output/libwaterkmark.a

LIBS+=-lsndfile
