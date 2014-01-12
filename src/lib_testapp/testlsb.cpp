#include "manager/WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/DummyOutput.h"
#include "watermark/LSBEncode.h"
#include "watermark/LSBDecode.h"
#include "watermarkdata/SimpleWatermarkData.h"

#include "TestHeader.h"

void encode()
{
	Parameters<short> conf;
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

	WatermarkManager manager(Input_p(input),
							 Output_p(output),
							 Watermark_p(new LSBEncode<short>(conf)),
							 WatermarkData_p(data));

	manager.execute();

	output->writeFile("out_test_lsb_encode.wav");
}

void decode()
{
	Parameters<short> conf;
	WatermarkData* data = new SimpleWatermarkData;

	auto input = new FileInput<short>("out_test_lsb_encode.wav", conf);
	auto output = new DummyOutput<short>(conf);

	WatermarkManager manager(Input_p(input),
							 Output_p(output),
							 Watermark_p(new LSBDecode<short>(conf)),
							 WatermarkData_p(data));
	manager.execute();

	// LIRE LA TAILLE
	data->readSizeFromBits();
	QCOMPARE(data->printBits(), std::string("1011001"));
}

void TestLSB()
{
	encode();
	decode();
}


