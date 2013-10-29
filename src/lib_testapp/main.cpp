#include <iostream>
#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "watermark/GainTest.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "fft/fftwmanager.h"
#include "watermark/bypass.h"
using namespace std;

void SpectralTest()
{
	Parameters conf;
	auto manager = new WatermarkManager(conf);

	auto input = new FileInput("test_file.raw", conf);
	auto output = new FileOutput(conf);

	auto fft_m = new FFTWManager(conf);
	auto fft_i = new FFTInputProxy(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy(output, fft_m, conf);

	auto algorithm = new BypassWatermark(conf);

	manager->_input.reset(fft_i);
	manager->_output.reset(fft_o);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeFile("out_test.wav");
}

void TemporalTest()
{
	Parameters conf;
	auto manager = new WatermarkManager(conf);

	auto input = new FileInput("test_file.raw", conf);
	auto output = new FileOutput(conf);

	auto algorithm = new GainTest(conf);

	manager->_input.reset(input);
	manager->_output.reset(output);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeFile("out_test.wav");
}

void TestFFTWManager()
{
	// Generer une sine
	Parameters conf;
	auto fft_m = new FFTWManager(conf);

	for(auto i = 0U; i < conf.bufferSize; ++i)
		fft_m->input()[i] = sin(440.0 * (2.0 * 3.1415) * i / 44100.0);

	fft_m->forward();
	fft_m->backward();

	for(auto i = 0U; i < conf.bufferSize; ++i)
		std::cerr << fft_m->input()[i] << "\t\t" << fft_m->output()[i] / conf.bufferSize << std::endl;
}

int main()
{
	SpectralTest();
	return 0;
}

