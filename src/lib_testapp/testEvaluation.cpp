#include "io/FileInput.h"
#include "io/FileOutput.h"

#include "io/BufferInput.h"
#include "io/BufferOutput.h"
#include "io/SilenceInput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "evaluation/BitError.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "manager/WatermarkManager.h"
#include "manager/BenchmarkManager.h"
#include "watermark/LSBEncode.h"
#include "watermark/LSBDecode.h"
#include "watermark/RLSBEncode.h"
#include "watermark/RLSBDecode.h"
#include "watermark/SSWEncode.h"
#include "watermark/SSWDecode.h"
#include <string>
#include <algorithm>
#include <random>
#include "benchmark/Amplify.h"
#include "benchmark/Convolution.h"
#include "benchmark/FFTAmplify.h"


#include "Parameters.h"
#include "Data.h"
#include "watermarkdata/WatermarkData.h"
#include "watermark/WatermarkBase.h"

void TestEvaluation()
{

    std::cout << "******** Algorithme de watermark : LSB ********" << std::endl;
    BitError<double, Amplify<double>, short, LSBEncode<short>, LSBDecode<short>> lsbAmplify;
    lsbAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : Amplify ====> False bits = " << lsbAmplify.getError() << std::endl;

    BitError<double, Convolution<double>, short, LSBEncode<short>, LSBDecode<short>> lsbConvolution;
    lsbConvolution.execute("input_mono.wav");
    std::cout << "Dégradation : Convolution ====> False bits = " << lsbConvolution.getError() << std::endl;

    BitError<double, FFTAmplify<double>, short, LSBEncode<short>, LSBDecode<short>> lsbFFTAmplify;
    lsbFFTAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : FFTAmplify ====> False bits = " << lsbFFTAmplify.getError() << std::endl;

    std::cout << "******** Algorithme de watermark : RLSB ********" << std::endl;
    BitError<double, Amplify<double>, short, RLSBEncode<short>, RLSBDecode<short>> rlsbAmplify;
    rlsbAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : Amplify ====> False bits = " << rlsbAmplify.getError() << std::endl;

    BitError<double, Convolution<double>, short, RLSBEncode<short>, RLSBDecode<short>> rlsbConvolution;
    rlsbConvolution.execute("input_mono.wav");
    std::cout << "Dégradation : Convolution ====> False bits = " << rlsbConvolution.getError() << std::endl;

    BitError<double, FFTAmplify<double>, short, RLSBEncode<short>, RLSBDecode<short>> rlsbFFTAmplify;
    rlsbFFTAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : FFTAmplify ====> False bits = " << rlsbFFTAmplify.getError() << std::endl;

    std::cout << "******** Algorithme de watermark : SSW ********" << std::endl;
    /*BitError<double, Amplify<double>, short, SSWEncode<short>, SSWDecode<short>> sswAmplify;
    sswAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : Amplify ====> False bits = " << sswAmplify.getError() << std::endl;

    BitError<double, Convolution<double>, short, SSWEncode<short>, SSWDecode<short>> sswConvolution;
    sswConvolution.execute("input_mono.wav");
    std::cout << "Dégradation : Convolution ====> False bits = " << sswConvolution.getError() << std::endl;

    BitError<double, FFTAmplify<double>, short, SSWEncode<short>, SSWDecode<short>> sswFFTAmplify;
    sswFFTAmplify.execute("input_mono.wav");
    std::cout << "Dégradation : FFTAmplify ====> False bits = " << sswFFTAmplify.getError() << std::endl;
    */


}
