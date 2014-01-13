#include "manager/WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "transform/FFTWManager.h"
#include "io/SilenceInput.h"
#include "io/DummyOutput.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "io/mcltproxy/MCLTInputProxy.h"
#include "io/mcltproxy/MCLTOutputProxy.h"

#include "TestHeader.h"

#include "watermark/DummyWatermark.h"

/***** Test du fonctionnement de la MCLT *****/
void TestMCLT()
{
	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(4);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);

	Parameters<double> conf;
	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());
	auto fft_i = Input_p(new FFTInputProxy<double>(input, fft_m, conf));
	auto fft_o = Output_p(new FFTOutputProxy<double>(output, fft_m, conf));

	auto mclt_i = Input_p(new MCLTInputProxy<double>(fft_i, conf));
	auto mclt_o = Output_p(new MCLTOutputProxy<double>(fft_o, conf));

	auto algorithm = new DummyWatermark<double>(conf);

	WatermarkManager manager(mclt_i,
							 mclt_o,
							 Watermark_p(algorithm),
							 WatermarkData_p(data));

	manager.execute();

	output->writeFile("out_test_mclt_mono.wav");
}

void TestTransform()
{
	TestMCLT();
}
