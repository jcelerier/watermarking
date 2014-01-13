#include "manager/WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/BufferInput.h"
#include "io/BufferOutput.h"
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
void TestSimpleMCLT()
{
	Parameters<double> conf;
	conf.bufferSize = 10;

	auto input = new BufferInput<double>(conf);

	double tem[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	double tab[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	input->readBuffer<double>(tab, 10, 1);
	auto output = new BufferOutput<double>(conf);

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
							 nullptr);

	manager.execute();

	output->writeOutBuffer(tab);
	for(int i = 0; i < 10; i++)
		std::cerr << tab[i] << " ";

	std::cerr << std::endl;
}

void TestMCLT()
{
	Parameters<double> conf;
	auto input = new FileInput<double>("input_mono.wav", new InputOLA<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputOLA<double>(conf), conf);

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
							 nullptr);

	manager.execute();

	output->writeFile("out_test_mclt_mono.wav");
}

void TestTransform()
{
	TestMCLT();
}
