CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(qfftspinboxplugin)
TEMPLATE    = lib

HEADERS     = qfftspinboxplugin.h
SOURCES     = qfftspinboxplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
DESTDIR = $$PWD/../../../output
INSTALLS    += target

include(qfftspinbox.pri)
