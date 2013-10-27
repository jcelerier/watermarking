QT       -= core gui

TARGET = waterkmark
TEMPLATE = lib
CONFIG += staticlib c++11
DESTDIR = $$PWD/../../output
LIBS += -lfftw3

contains(QMAKE_TARGET.arch, 64):{
msvc:QMAKE_CXXFLAGS_RELEASE += -openmp -arch:AVX
else:QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -D_GLIBCXX_PARALLEL
}
contains(QMAKE_TARGET.arch, arm):{
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
}
QMAKE_CXXFLAGS +=  -Wall -pedantic -Wextra -Weffc++  -Wall -Wcast-align  -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion  -Wdisabled-optimization    -Wformat  -Wformat=1  -Wformat-nonliteral -Wformat-security   -Wformat-y2k  -Wimport  -Winit-self  -Winline  -Winvalid-pch    -Wunsafe-loop-optimizations  -Wmissing-braces  -Wmissing-field-initializers -Wmissing-format-attribute    -Wmissing-include-dirs -Wmissing-noreturn  -Wpacked  -Wparentheses  -Wpointer-arith  -Wredundant-decls -Wreturn-type  -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing=3 -Wswitch  -Wswitch-default  -Wswitch-enum -Wtrigraphs  -Wuninitialized  -Wunknown-pragmas  -Wunreachable-code -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter  -Wunused-value  -Wunused-variable  -Wvariadic-macros  -Wvolatile-register-var  -Wwrite-strings
QMAKE_CXXFLAGS += -Wno-unknown-pragmas
lessThan(QT_MAJOR_VERSION, 5) {
message("hey qt4")
QMAKE_CXXFLAGS += -std=c++11
}


BASEPATH = ../build/libnoisered

CONFIG(debug, debug|release) {
	BUILDDIR = $${BASEPATH}/debug
} else {
	BUILDDIR = $${BASEPATH}/release
}
OBJECTS_DIR = $${BUILDDIR}/obj
MOC_DIR = $${BUILDDIR}/moc
RCC_DIR = $${BUILDDIR}/rcc
UI_DIR = $${BUILDDIR}/ui



SOURCES += \
	eval.cpp \
	mathutils/math_util.cpp \
	fft/fftmanager.cpp \
	fft/fftwmanager.cpp \
    watermark_manager.cpp \
    watermark/bypass.cpp

HEADERS += \
	eval.h \
	subtraction/algorithms.h \
	mathutils/math_util.h \
	fft/fftmanager.h \
	fft/fftwmanager.h \
    watermark/bypass.h \
    watermark_manager.h \
    watermark/spectralwatermarkbase.h \
    watermark/watermarkbase.h
