#pragma once
#include <memory>
#include <complex>
class WatermarkBase;
class InputManagerBase;
class OutputManagerBase;
class FFTManager;

typedef std::shared_ptr<WatermarkBase> Watermark_p;
typedef std::shared_ptr<OutputManagerBase> Output_p;
typedef std::shared_ptr<InputManagerBase> Input_p;
typedef std::shared_ptr<FFTManager> FFT_p;

struct Parameters
{
		using data_type = double;
		using size_type = long unsigned int;
		using complex_type = std::complex<data_type>;

		size_type samplingRate = 44100;
		size_type bufferSize = 512;
};
