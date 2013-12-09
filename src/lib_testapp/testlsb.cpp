#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/DummyOutput.h"
#include "watermark/LSBEncode.h"
#include "watermark/LSBDecode.h"
#include "watermarkdata/SimpleWatermarkData.h"

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
	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(7);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(false);
	data->setNextBit(true);

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
	WatermarkData* data = new SimpleWatermarkData;

    auto input = new FileInput<short>("out_test_lsb_encode.wav", conf);
	auto output = new DummyOutput<short>(conf);

    auto algorithm = new LSBDecode<short>(conf);

    manager.data.reset(data);
    manager.input.reset(input);
    manager.output.reset(output);
    manager.algorithm.reset(algorithm);

    manager.execute();

	// LIRE LA TAILLE
	data->readSizeFromBits();
	data->printBits();
}
