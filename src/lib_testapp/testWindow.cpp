#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/copystyle/InputOLA.h"
#include "io/copystyle/OutputOLA.h"
#include "benchmark/Dummy.h"
#include "manager/BenchmarkManager.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "transform/FFTWManager.h"

#include "io/fftproxy/window/RectWindow.h"
#include "io/fftproxy/window/BartlettWindow.h"

#include "io/fftproxy/window/HighOrderCosineWindow.h"

void windFFT(std::string name, WindowBase<double>* window, Parameters<double> conf)
{
	auto iola = new InputOLA<double>(conf, window->acceptableFactors()[0]);
	auto oola = new OutputOLA<double>(conf, window->acceptableFactors()[0]);

	window->generate(iola->copiedSamples());

	/*
	auto input = new SilenceInput<double>(iola, conf);
	input->silence(4096, 1, 1.0);
	*/

	auto input = new FileInput<double>("input_mono.wav", iola, conf);
	auto output = new FileOutput<double>(oola, conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());

	auto fft_i = new FFTInputProxy<double>(window, input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	BenchmarkManager manager(Input_p(fft_i),
							 Output_p(fft_o),
							 Benchmark_p(new Dummy<double>(conf)));

	manager.execute();
	output->writeFile(("out_test_window_" + name + ".wav").c_str());
}

void testWindow()
{
	Parameters<double> conf;
	windFFT("Rect", new RectWindow<double>, conf);
	windFFT("Bartlett", new BartlettWindow<double>, conf);
	windFFT("Hann", CosineWindows::Hann<double>(), conf);
	windFFT("Hamming", CosineWindows::Hamming<double>(), conf);

	// Bof pour resynthèse
	windFFT("Blackman", CosineWindows::Blackman<double>(), conf);
	windFFT("BlackmanHarris", CosineWindows::BlackmanHarris<double>(), conf);
	windFFT("Nuttal3", CosineWindows::Nuttal3<double>(), conf);
	windFFT("SFT3F", CosineWindows::SFT3F<double>(), conf);
}
