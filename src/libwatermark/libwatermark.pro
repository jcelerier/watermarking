QT       -= core gui

TARGET = waterkmark
TEMPLATE = lib
CONFIG += staticlib c++11
DESTDIR = $$PWD/../../output
LIBS += -lfftw3

contains(QMAKE_TARGET.arch, 64):{
msvc:QMAKE_CXXFLAGS_RELEASE += -openmp -arch:AVX
else:QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -D_GLIBCXX_PARALLEL
}
contains(QMAKE_TARGET.arch, arm):{
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
}
QMAKE_CXXFLAGS +=  -Wall -pedantic -Wextra -Weffc++  -Wall -Wcast-align  -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion  -Wdisabled-optimization    -Wformat  -Wformat=1  -Wformat-nonliteral -Wformat-security   -Wformat-y2k  -Wimport  -Winit-self  -Winline  -Winvalid-pch    -Wunsafe-loop-optimizations  -Wmissing-braces  -Wmissing-field-initializers -Wmissing-format-attribute    -Wmissing-include-dirs -Wmissing-noreturn  -Wpacked  -Wparentheses  -Wpointer-arith  -Wredundant-decls -Wreturn-type  -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing=3 -Wswitch  -Wswitch-default  -Wswitch-enum -Wtrigraphs  -Wuninitialized  -Wunknown-pragmas  -Wunreachable-code -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter  -Wunused-value  -Wunused-variable  -Wvariadic-macros  -Wvolatile-register-var  -Wwrite-strings
QMAKE_CXXFLAGS += -Wno-unknown-pragmas -fopenmp
lessThan(QT_MAJOR_VERSION, 5) {
message("hey qt4")
QMAKE_CXXFLAGS += -std=c++11
}


HEADERS += \
        mathutils/math_util.h \
    io/IOManagerBase.h \
    io/InputManagerBase.h \
    io/OutputManagerBase.h \
    io/copystyle/InputSimple.h \
    io/copystyle/InputOLA.h \
    io/copystyle/OutputSimple.h \
    io/copystyle/OutputOLA.h \
    io/FileInput.h \
    io/BufferInput.h \
    io/BufferOutput.h \
    io/copystyle/CopyStyle.h \
    Parameters.h \
    Data.h \
    io/fft/FFTProxy.h \
    watermark/GainTest.h \
    io/FileOutput.h \
    io/fftproxy/FFTInputProxy.h \
    io/fftproxy/FFTProxy.h \
    io/fftproxy/FFTOutputProxy.h \
    WatermarkManager.h \
    watermark/SpectralGain.h \
    watermark/WatermarkBase.h \
    watermark/SpectralWatermarkBase.h \
    io/copystyle/InputCopy.h \
    io/copystyle/OutputCopy.h \
    fft/FFTManager.h \
    fft/FFTWManager.h \
    io/fftproxy/window/WindowBase.h \
    io/fftproxy/window/HannWindow.h \
    io/fftproxy/window/RectWindow.h \
    io/fftproxy/window/HammingWindow.h \
    io/SilenceInput.h \
    io/fftproxy/window/BlackmanWindow.h \
    io/fftproxy/window/BartlettWindow.h \
    io/gnuplot/gnuplot_i.h \
    io/GnuplotOutput.h \
    io/GnuplotFFTOutput.h \
    watermark/LSBEncode.h \
    watermark/LSBDecode.h \
    WatermarkData.h \
    watermark/SSWEncode.h \
    mathutils/MCLT.h \
    io/mcltproxy/MCLTInputProxy.h \
    io/mcltproxy/MCLTProxy.h \
    io/mcltproxy/MCLTOutputProxy.h \
    mathutils/ssw_utils.h \
    watermark/SSWDecode.h
