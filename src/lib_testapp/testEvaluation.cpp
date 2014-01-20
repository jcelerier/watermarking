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
#include <string>
#include <algorithm>
#include <random>
#include "benchmark/Amplify.h"
#include "watermark/LSBDecode.h"

#include "Parameters.h"
#include "Data.h"
#include "watermarkdata/WatermarkData.h"
#include "watermark/WatermarkBase.h"

void TestEvaluation()
{
    BitError<double, Amplify<double>, short, LSBEncode<short>, LSBDecode<short>> b;

	b.execute("input_mono.wav");
    //std::cout << "False bits = " << b.getError() << std::endl;
}
