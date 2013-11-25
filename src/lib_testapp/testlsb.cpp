#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "watermark/LSBEncode.h"
#include "watermark/LSBDecode.h"

void encode();
void decode();
void TestLSB()
{
    encode();
    decode();
}


void encode()
{
    Parameters<short> conf;
    WatermarkManager<short> manager(conf);
    WatermarkData* data = new WatermarkData;
    data->bits.push_back(true);
    data->bits.push_back(false);
    data->bits.push_back(true);
    data->bits.push_back(true);

    auto input = new FileInput<short>("input_mono.wav", conf);
    auto output = new FileOutput<short>(conf);

    auto algorithm = new LSBEncode<short>(conf);

    manager.data.reset(data);
    manager.input.reset(input);
    manager.output.reset(output);
    manager.algorithm.reset(algorithm);

    manager.execute();

    output->writeFile("out_test_lsb_encode.wav");
}

void decode()
{
    Parameters<short> conf;
    WatermarkManager<short> manager(conf);
    WatermarkData* data = new WatermarkData;

    auto input = new FileInput<short>("out_test_lsb_encode.wav", conf);
    auto output = new FileOutput<short>(conf);

    auto algorithm = new LSBDecode<short>(conf);

    manager.data.reset(data);
    manager.input.reset(input);
    manager.output.reset(output);
    manager.algorithm.reset(algorithm);

    manager.execute();

    std::cerr << data->bits[0] << data->bits[1] << data->bits[2] << data->bits[3];

}
