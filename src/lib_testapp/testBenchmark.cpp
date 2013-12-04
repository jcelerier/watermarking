#include "BenchmarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "benchmark/Amplify.h"

void bench();
void TestBenchmark()
{
    bench();
}


void bench()
{
	Parameters<double> conf;
	BenchmarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	auto algorithm = new Amplify<double>(conf);
	algorithm->setGain(0.5);

    manager.input.reset(input);
    manager.output.reset(output);
    manager.algorithm.reset(algorithm);

    manager.execute();

	output->writeFile("out_test_bench.wav");
}

