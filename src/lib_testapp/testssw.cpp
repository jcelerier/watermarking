#include "manager/WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "transform/FFTWManager.h"
#include "watermark/SSWEncode.h"
#include "watermark/SSWDecode.h"
#include "io/BufferInput.h"
#include "io/BufferOutput.h"
#include "io/SilenceInput.h"
#include "io/DummyOutput.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "timeadapter/Every.h"
#include "io/GnuplotFFTOutput.h"
#include "io/GnuplotOutput.h"
#include "io/mcltproxy/MCLTInputProxy.h"
#include "io/mcltproxy/MCLTOutputProxy.h"
#include "watermark/DummyWatermark.h"
#include "mathutils/ssw_utils.h"
#include "TestHeader.h"

void sswencode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude);
void sswdecode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold);
void sswencode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude);
void sswdecode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold);

void TestSSW()
{
	Parameters<double> conf;
	conf.bufferSize = 512U;

	int SeqSize = 50;
	double watermarkAmplitude = 10;
	double threshold = 0.5;

	std::vector<int> PNSequence = SSWUtil::generatePNSequence(SeqSize);
	auto FreqRange = SSWUtil::generateFrequencyRange(PNSequence.size(), conf);

	sswencode(PNSequence, FreqRange, conf, watermarkAmplitude);
	sswdecode(PNSequence, FreqRange, conf, watermarkAmplitude, threshold);
/*
	std::cerr << "Test avec MCLT" << std::endl;
	sswencode_mclt(PNSequence, FreqRange, conf, watermarkAmplitude);
	sswdecode_mclt(PNSequence, FreqRange, conf, watermarkAmplitude, threshold);
*/}

void sswencode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude)
{
	std::cout << std::endl << "Encodage..." << std::endl;
	// Données à watermarker
	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(4);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);

	auto input = new FileInput<double>("solo.wav", new InputSimple<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputSimple<double>(conf), conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	auto algorithm = Watermark_p(
			new SSWEncode<double>(conf, PNSequence, FreqRange, watermarkAmplitude));

	WatermarkManager manager(Input_p(fft_i),
				 Output_p(fft_o),
				 algorithm,
				 WatermarkData_p(data));

	manager.execute();

	output->writeFile("out_test_ssw_mono.wav");

	std::cout << "Encodé." << std::endl;
}

void sswdecode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold)
{
	std::cout << "Décodage..." << std::endl;

	WatermarkData* data = new SimpleWatermarkData();

	auto input = new FileInput<double>("out_test_ssw_mono.wav", conf);
	//auto input = new FileInput<double>("music.wav", conf);
	auto output = new DummyOutput<double>(conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	auto algorithm = Watermark_p(new SSWDecode<double>(conf, PNSequence, FreqRange, watermarkAmplitude, threshold));

	WatermarkManager manager(Input_p(fft_i),
							 Output_p(fft_o),
							 algorithm,
							 WatermarkData_p(data));

	manager.execute();

	data->readSizeFromBits();

	QCOMPARE(data->printBits(), std::string("1011"));
}


// Valentin :
// Normalement la MCLT marche, en tout cas elle donne les mêmes résultats que sur Matlab.
// Par contre elle applique un fenêtrage aux données, et pour reconstruire le signal
// il est nécessaire de faire le process en overlap (facteur d'1/2).
// Donc je pense qu'avant de faire marcher la SSW avec la MCLT, il est nécessaire
// de s'assurer que ça passe bien avec juste de l'overlap (cf. InputOLA, OutputOLA)
// dans un autre test.
void sswencode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude)
{
	std::cout << std::endl << "Encodage..." << std::endl;

	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(4);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);

	auto input = new FileInput<double>("solo.wav", new InputOLA<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputOLA<double>(conf), conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf)); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels((unsigned int) input->channels()); // important.
	auto fft_i = Input_p(new FFTInputProxy<double>(input, fft_m, conf));
	auto fft_o = Output_p(new FFTOutputProxy<double>(output, fft_m, conf));

	auto mclt_i = new MCLTInputProxy<double>(fft_i, conf);
	auto mclt_o = new MCLTOutputProxy<double>(fft_o, conf);

	auto algorithm = Watermark_p(
			new SSWEncode<double>(conf, PNSequence, FreqRange, watermarkAmplitude));

	WatermarkManager manager(Input_p(mclt_i),
				 Output_p(mclt_o),
				 algorithm,
				 WatermarkData_p(data));

	manager.execute();

	output->writeFile("out_test_ssw_mclt_mono.wav");

	std::cout << "Encodé." << std::endl;
}

void sswdecode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold)
{
	std::cout << "Décodage..." << std::endl;

	WatermarkData* data = new SimpleWatermarkData();

	auto input = new FileInput<double>("out_test_ssw_mclt_mono.wav", new InputOLA<double>(conf), conf);
	auto output = new DummyOutput<double>(new OutputOLA<double>(conf), conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());
	auto fft_i = Input_p(new FFTInputProxy<double>(input, fft_m, conf));
	auto fft_o = Output_p(new FFTOutputProxy<double>(output, fft_m, conf));

	auto mclt_i = new MCLTInputProxy<double>(fft_i, conf);
	auto mclt_o = new MCLTOutputProxy<double>(fft_o, conf);

	auto algorithm = Watermark_p(new SSWDecode<double>(conf, PNSequence, FreqRange, watermarkAmplitude, threshold));

	WatermarkManager manager(Input_p(mclt_i),
				 Output_p(mclt_o),
				 algorithm,
				 WatermarkData_p(data));

	manager.execute();

	data->readSizeFromBits();

	QCOMPARE(data->printBits(), std::string("1011"));
}
