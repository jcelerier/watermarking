#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/DummyOutput.h"
#include "io/InputMultiplexer.h"
#include "io/copystyle/InputOLA.h"
#include "io/copystyle/OutputOLA.h"
#include "benchmark/Dummy.h"
#include "benchmark/ComputeRMS.h"
#include "manager/BenchmarkManager.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "transform/FFTWManager.h"

#include "io/fftproxy/window/RectWindow.h"
#include "io/fftproxy/window/BartlettWindow.h"

#include "io/fftproxy/window/HighOrderCosineWindow.h"

#include "TestHeader.h"

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


void windFFTCompare(std::string name, WindowBase<double>* window, Parameters<double> conf)
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


	auto binput1 = Input_p(new FileInput<double>("input_mono.wav", conf));
	auto binput2 = Input_p(new FileInput<double>(("out_test_window_" + name + ".wav").c_str(), conf));

	auto binput = Input_p(new InputMultiplexer<double>(binput1, binput2, conf));
	auto boutput = Output_p(new DummyOutput<double>(conf));

	auto balgo = new ComputeRMS<double>(conf);
	BenchmarkManager m2(binput,
						boutput,
						Benchmark_p(balgo));

	m2.execute();

	double rms = balgo->rms / (input->frames() / double(conf.bufferSize));
	std::cerr << name << " RMS Difference : " << rms << std::endl;
	QVERIFY(rms < 1);
}

void testWindow()
{
	Parameters<double> conf;
	windFFTCompare("Rect", new RectWindow<double>, conf);
	/*windFFTCompare("Bartlett", new BartlettWindow<double>, conf);
	windFFTCompare("Hann", CosineWindows::Hann<double>(), conf);
	windFFTCompare("Hamming", CosineWindows::Hamming<double>(), conf);

	// Bof pour resynthèse
	windFFTCompare("Blackman", CosineWindows::Blackman<double>(), conf);
	windFFTCompare("BlackmanHarris", CosineWindows::BlackmanHarris<double>(), conf);
	windFFTCompare("Nuttal3", CosineWindows::Nuttal3<double>(), conf);
	windFFTCompare("SFT3F", CosineWindows::SFT3F<double>(), conf);*/
}
