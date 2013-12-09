#include "BenchmarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/SilenceInput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "benchmark/Convolution.h"
#include "benchmark/AddBrumm.h"
#include "benchmark/AddWhiteNoise.h"
#include "benchmark/Amplify.h"
#include "benchmark/Exchange.h"
#include "benchmark/Invert.h"
#include "benchmark/Smooth.h"
#include "benchmark/ZeroCross.h"


void addBrumm()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new SilenceInput<double>(conf);
	input->silence(1024 * conf.bufferSize, 1);
	auto output = new FileOutput<double>(conf);

	auto algorithm = new AddBrumm<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_brumm.wav");
}

void convolution()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("in_noise_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Convolution<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_lowpass.wav");
}

void amplify()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Convolution<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_amplify.wav");
}

void exchange()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Exchange<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_exchange.wav");
}

void smooth()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Smooth<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_smooth.wav");
}

void invert()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new Invert<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_invert.wav");
}

void zerocross()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputFilter<double>(11, conf), conf);
	auto output = new FileOutput<double>(new OutputFilter<double>(11, conf), conf);

	auto algorithm = new ZeroCross<double>(conf);

	manager.input.reset(input);
	manager.output.reset(output);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_zerocross.wav");
}

void TestBenchmark()
{
	addBrumm();
	amplify();
	convolution();
	exchange();
	smooth();
	invert();
	zerocross();
}
