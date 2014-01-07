#include "manager/WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/DummyOutput.h"
#include "watermark/RLSBEncode.h"
#include "watermark/RLSBDecode.h"
#include "watermarkdata/SimpleWatermarkData.h"

#include "TestHeader.h"

void rencode()
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
                             Watermark_p(new RLSBEncode<short>(conf)),
							 WatermarkData_p(data));

	manager.execute();

    output->writeFile("out_test_rlsb_encode.wav");
}

void rdecode()
{
	Parameters<short> conf;
	WatermarkData* data = new SimpleWatermarkData;

    auto input = new FileInput<short>("out_test_rlsb_encode.wav", conf);
	auto output = new DummyOutput<short>(conf);

	WatermarkManager manager(Input_p(input),
							 Output_p(output),
                             Watermark_p(new RLSBDecode<short>(conf)),
							 WatermarkData_p(data));
	manager.execute();

	// LIRE LA TAILLE
	data->readSizeFromBits();
	QVERIFY(data->printBits() == "1011001");
}

void TestRLSB()
{
    rencode();
    rdecode();
}


