#include <iostream>
#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "watermark/GainTest.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "fft/fftwmanager.h"
#include "watermark/SpectralGain.h"

using namespace std;

void SpectralTest()
{
	Parameters<double> conf;
	auto manager = new WatermarkManager<double>(conf);

	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	auto fft_m = new FFTWManager<double>(conf);
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	auto algorithm = new SpectralGain<double>(conf);

	manager->_input.reset(fft_i);
	manager->_output.reset(fft_o);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeFile("out_test.wav");
}

void TemporalTest()
{
	Parameters<double> conf;
	auto manager = new WatermarkManager<double>(conf);

	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	auto algorithm = new GainTest<double>(conf);

	manager->_input.reset(input);
	manager->_output.reset(output);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeFile("out_test.wav");
}

void TestFFTWManager()
{
	// Generer une sine
	Parameters<double> conf;
	auto fft_m = new FFTWManager<double>(conf);

	for(auto i = 0U; i < conf.bufferSize; ++i)
		fft_m->input()[i] = sin(440.0 * (2.0 * 3.1415) * i / 44100.0);

	fft_m->forward();
	fft_m->backward();

	for(auto i = 0U; i < conf.bufferSize; ++i)
		std::cerr << fft_m->input()[i] << "\t\t" << fft_m->output()[i] / conf.bufferSize << std::endl;
}

int main()
{
	TemporalTest();
	return 0;
}

