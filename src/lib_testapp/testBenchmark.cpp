#include "BenchmarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/SilenceInput.h"
#include "benchmark/Amplify.h"
#include "benchmark/AddBrumm.h"

void bench();
void TestBenchmark()
{
	bench();
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

