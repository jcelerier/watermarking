#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/copystyle/InputSimple.h"
#include "io/copystyle/InputOLA.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputSimple.h"
#include "io/copystyle/OutputOLA.h"
#include "io/copystyle/OutputFilter.h"

#include "benchmark/Dummy.h"
#include "benchmark/Amplify.h"

#include "manager/BenchmarkManager.h"

#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "transform/FFTWManager.h"

template<typename T>
void implStd(std::string name, InputCopy<T>* i, OutputCopy<T>* o, Parameters<T> conf)
{
	auto output = new FileOutput<T>(o, conf);

	BenchmarkManager manager(Input_p(new FileInput<T>("input_mono.wav", i, conf)),
							 Output_p(output),
							 Benchmark_p(new Amplify<T>(conf)));

	manager.execute();
	output->writeFile(("out_test_copy_" + name + ".wav").c_str());
}

void implFFT(std::string name, InputCopy<double>* i, OutputCopy<double>* o, Parameters<double> conf)
{
	auto input = new FileInput<double>("input_mono.wav", i, conf);
	auto output = new FileOutput<double>(o, conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());

	auto fft_i = new FFTInputProxy<double>(new RectWindow<double>,
										   input,
										   fft_m,
										   conf);

	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	BenchmarkManager manager(Input_p(fft_i),
							 Output_p(fft_o),
							 Benchmark_p(new Dummy<double>(conf)));

	manager.execute();
	output->writeFile(("out_test_copy_fft_" + name + ".wav").c_str());
}


void testCopy()
{
	Parameters<double> conf;
	implStd<double>("simple_dbl", new InputSimple<double>(conf), new OutputSimple<double>(conf), conf);
	implStd<double>("ola_dbl", new InputOLA<double>(conf), new OutputOLA<double>(conf), conf);
	implStd<double>("filter_dbl", new InputFilter<double>(20, conf), new OutputFilter<double>(20, conf), conf);

	implFFT("simple_dbl", new InputSimple<double>(conf), new OutputSimple<double>(conf), conf);
	implFFT("ola_dbl", new InputOLA<double>(conf), new OutputOLA<double>(conf), conf);
	implFFT("filter_dbl", new InputFilter<double>(20, conf), new OutputFilter<double>(20, conf), conf);
}
