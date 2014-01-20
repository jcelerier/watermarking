#include "manager/BenchmarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/SilenceInput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "io/copystyle/InputOLA.h"
#include "io/copystyle/OutputOLA.h"
#include "benchmark/Convolution.h"
#include "benchmark/AddBrumm.h"
#include "benchmark/AddWhiteNoise.h"
#include "benchmark/Amplify.h"
#include "benchmark/Exchange.h"
#include "benchmark/Invert.h"
#include "benchmark/FFTAmplify.h"
#include "benchmark/Smooth.h"
#include "benchmark/ZeroCross.h"
#include "benchmark/Stat1.h"
#include "benchmark/FFTNoise.h"
#include "../libwatermark/watermark/mask.h"

#include "transform/FFTWManager.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"

void DoubleTestBase(Benchmark_p b, Parameters<double> conf)
{
	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	BenchmarkManager manager(Input_p(input),
							 Output_p(output),
							 b);

	manager.execute();

	output->writeFile(std::string("out_test_" + b->_name + ".wav").c_str());
}

void FFTTestBase(Benchmark_p b, Parameters<double> conf)
{
	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());

	auto fft_i = new FFTInputProxy<double>(new RectWindow<double>,
										   input,
										   fft_m,
										   conf);

	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	BenchmarkManager manager(Input_p(fft_i),
							 Output_p(fft_o),
							 b);

	manager.execute();

	output->writeFile(std::string("out_test_" + b->_name + ".wav").c_str());
}


void TestOLA()
{
	Parameters<double> conf;

	auto input = new FileInput<double>("input_mono.wav", new InputOLA<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputOLA<double>(conf), conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf)); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels((unsigned int) input->channels()); // important.

	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	BenchmarkManager manager(Input_p(fft_i),
							 Output_p(fft_o),
							 Benchmark_p(new Amplify<double>(conf)));

	manager.execute();

	output->writeFile("out_test_ola.wav");
}



void convolution()
{
	Parameters<double> conf;

	auto input = new FileInput<double>("in_noise_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	BenchmarkManager manager(Input_p(input),
							 Output_p(output),
							 Benchmark_p(new Convolution<double>(conf)));

	manager.execute();

	output->writeFile("out_test_lowpass.wav");
}


void TestBenchmark()
{
	Parameters<double> conf;
	DoubleTestBase(Benchmark_p(new AddBrumm<double>(conf)), conf);
	DoubleTestBase(Benchmark_p(new Amplify<double>(conf)), conf);
	DoubleTestBase(Benchmark_p(new Exchange<double>(conf)), conf);
	DoubleTestBase(Benchmark_p(new Smooth<double>(conf)), conf);
	DoubleTestBase(Benchmark_p(new Invert<double>(conf)), conf);
	DoubleTestBase(Benchmark_p(new ZeroCross<double>(conf)), conf);
	DoubleTestBase(Benchmark_p(new Stat1<double>(conf)), conf);

	FFTTestBase(Benchmark_p(new FFTAmplify<double>(conf)), conf);
	FFTTestBase(Benchmark_p(new FFTNoise<double>(conf)), conf);

	convolution();

    Mask::calcul();

}
