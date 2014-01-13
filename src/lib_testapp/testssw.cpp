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

#include "mathutils/ssw_utils.h"
#include "TestHeader.h"

void sswencode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude);
void sswdecode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold);
void sswencode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude);
void sswdecode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold);
void TestMCLT();
#include "watermark/DummyWatermark.h"
void TestSSW()
{
	int SeqSize = 50;
	double watermarkAmplitude = 10;
	double threshold = 0.5;

	std::cout << std::endl;
	std::vector<int> PNSequence = SSWUtil::generatePNSequence(SeqSize);
	
	for (int i = 0; i < PNSequence.size(); i++) {
		std::cout << PNSequence[i] << " ";
	}
	std::cout << std::endl;
	
	
	std::vector<double> amplifiedPN;
	for (auto i = 0U; i < PNSequence.size(); i++) {
		amplifiedPN.push_back(watermarkAmplitude * (double) PNSequence[i]);
	}

	// Instanciation des paramètres
	Parameters<double> conf;
	conf.bufferSize = 512U;

	//double norm = MathUtil::norm_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), amplifiedPN.size());

	//std::cout << "Norme de PN : " << norm << std::endl;

	auto FreqRange = SSWUtil::generateFrequencyRange(PNSequence.size(), conf);

	for(auto i = 0U; i < FreqRange.size(); i++) std::cerr << FreqRange[i] << " ";
	sswencode(PNSequence, FreqRange, conf, watermarkAmplitude);
	sswdecode(PNSequence, FreqRange, conf, watermarkAmplitude, threshold);
	//sswencode_mclt(PNSequence, FreqRange, conf, watermarkAmplitude);
	//sswdecode_mclt(PNSequence, FreqRange, conf, watermarkAmplitude, threshold);
}

void sswencode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude)
{
	std::cout << std::endl << "Encodage..." << std::endl;

	//manager.timeAdapter = (TimeAdapter_p) new Every;

	// Données à watermarker
	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(4);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);

	// Instanciation du mode d'entrée et de sortie
	//auto input = new FileInput<double>("input_mono.wav", new InputSimple<double>(conf), conf);
	auto input = new FileInput<double>("solo.wav", new InputSimple<double>(conf), conf);
//	auto input = new SilenceInput<double>(conf);
//	input->silence(2048, 1);

	auto output = new FileOutput<double>(new OutputSimple<double>(conf), conf);

	// Comme c'est spectral on fait passer les entrées et sorties par un "filtre" qui va appliquer la FFT
	// Il est important que les proxy d'entrée et de sortie utilisent la même "implémentation" de FFT.
	FFT_p<double> fft_m(new FFTWManager<double>(conf)); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels((unsigned int) input->channels()); // important.
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	// L'algo de watermarking à utiliser (ici c'est juste du gain, pas de watermark)
	//auto algorithm = new SpectralGain<double>(conf);

	auto algorithm = Watermark_p(
			new SSWEncode<double>(conf, PNSequence, FreqRange, watermarkAmplitude));

	// On définit tout ce petit monde. Ce sont des smart_ptr d'ou le .reset. Avantage : pas besoin de faire de delete.
	WatermarkManager manager(Input_p(fft_i),
				 Output_p(fft_o),
				 algorithm,
				 WatermarkData_p(data));

	// On fait tourner l'algo
	manager.execute();

	// On écrit dans un fichier de sortie.
	output->writeFile("out_test_ssw_mono.wav");

	std::cout << "Encodé." << std::endl;
}

void sswdecode(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude, double threshold)
{
	std::cout << "Décodage..." << std::endl;

	WatermarkData* data = new SimpleWatermarkData();

	auto input = new FileInput<double>("out_test_ssw_mono.wav", conf);
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

void sswencode_mclt(std::vector<int> & PNSequence, std::vector<unsigned int> & FreqRange, Parameters<double> & conf, double watermarkAmplitude)
{
	std::cout << std::endl << "Encodage..." << std::endl;

	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(4);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);

	auto input = new FileInput<double>("solo.wav", new InputSimple<double>(conf), conf);

	auto output = new FileOutput<double>(new OutputSimple<double>(conf), conf);

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

	auto input = new FileInput<double>("out_test_ssw_mclt_mono.wav", conf);
	auto output = new DummyOutput<double>(conf);

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

/***** Test du fonctionnement de la MCLT *****/
void TestMCLT()
{
	/*
	Parameters<double> conf;
	WatermarkManager<double> manager(conf);

	auto input = new FileInput<double>("input_mono.wav", new InputSimple<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputSimple<double>(conf), conf);

	FFT_p<double> fft_m(new FFTWManager<double>(conf));
	fft_m->setChannels((unsigned int) input->channels());
	auto fft_i = Input_p<double>(new FFTInputProxy<double>(input, fft_m, conf));
	auto fft_o = Output_p<double>(new FFTOutputProxy<double>(output, fft_m, conf));

	auto mclt_i = Input_p<double>(new MCLTInputProxy<double>(fft_i, conf));
	auto mclt_o = Output_p<double>(new MCLTOutputProxy<double>(fft_o, conf));

	auto algorithm = new SpectralGain<double>(conf);

	manager.input.swap(mclt_i);
	manager.output.swap(mclt_o);
	manager.algorithm.reset(algorithm);

	manager.execute();

	output->writeFile("out_test_mclt_mono.wav");
*/}
