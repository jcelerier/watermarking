#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "benchmark/AddBrumm.h"

void bench();
void TestBenchmark()
{
    bench();
}


void bench()
{
    Parameters<short> conf;
    WatermarkManager<short> manager(conf);

    auto input = new FileInput<short>("input_mono.wav", conf);
    auto output = new FileOutput<short>(conf);

    auto algorithm = new AddBrumm<short>(conf);

    manager.input.reset(input);
    manager.output.reset(output);
    manager.algorithm.reset(algorithm);

    manager.execute();

    output->writeFile("out_test_lsb_encode.wav");
}

