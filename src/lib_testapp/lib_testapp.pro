TEMPLATE = app
CONFIG  += console c++11
QT      = core testlib

SOURCES += main.cpp \
    testlsb.cpp \
    testssw.cpp \
    testBenchmark.cpp \
    testCopy.cpp \
    testWindow.cpp \
    testrlsb.cpp
DESTDIR = $$PWD/../../output

contains(QMAKE_TARGET.arch, 64):{
msvc:QMAKE_CXXFLAGS_RELEASE += -openmp -arch:AVX
else:QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -D_GLIBCXX_PARALLEL
}
contains(QMAKE_TARGET.arch, arm):{
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
}


QMAKE_CXXFLAGS += -Wall -pedantic -Wextra -Weffc++  -Wall -Wcast-align  -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion  -Wdisabled-optimization    -Wformat  -Wformat=1  -Wformat-nonliteral -Wformat-security   -Wformat-y2k  -Wimport  -Winit-self  -Winline  -Winvalid-pch    -Wunsafe-loop-optimizations  -Wmissing-braces  -Wmissing-field-initializers -Wmissing-format-attribute    -Wmissing-include-dirs -Wmissing-noreturn  -Wpacked  -Wparentheses  -Wpointer-arith  -Wredundant-decls -Wreturn-type  -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing=3 -Wswitch  -Wswitch-default  -Wswitch-enum -Wtrigraphs  -Wuninitialized  -Wunknown-pragmas  -Wunreachable-code -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter  -Wunused-value  -Wunused-variable  -Wvariadic-macros  -Wvolatile-register-var  -Wwrite-strings
QMAKE_CXXFLAGS += -Wno-unknown-pragmas -Wno-inline -Wno-conversion

lessThan(QT_MAJOR_VERSION, 5) {
message("Warning: Qt4 used. Consider switching to Qt5")
QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += $$PWD/../libwatermark
DEPENDPATH += $$PWD/../libwatermark
macx {
 LIBS += -L/usr/local/Cellar/libsndfile/1.0.25/lib/ -L/usr/local/Cellar/fftw/3.3.3/lib/
}
LIBS+=-lsndfile  -lfftw3

OTHER_FILES += \
    Vieux_tests.txt

HEADERS += \
    sndfile.hh \
    sndfile.h \
    TestHeader.h
