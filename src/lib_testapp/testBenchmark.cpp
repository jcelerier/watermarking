#include "BenchmarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/SilenceInput.h"
#include "io/copystyle/InputFilter.h"
#include "io/copystyle/OutputFilter.h"
#include "benchmark/Convolution.h"
#include "benchmark/AddBrumm.h"

void bench();
void filter();
void TestBenchmark()
{
	filter();
	//bench();
}


void bench()
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

	output->writeFile("out_test_bench.wav");
}

void filter()
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
