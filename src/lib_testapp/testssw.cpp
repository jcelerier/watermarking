#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "fft/FFTWManager.h"
#include "watermark/SSWEncode.h"
#include "watermark/SSWDecode.h"
#include "io/BufferInput.h"
#include "io/BufferOutput.h"
#include "io/SilenceInput.h"
#include "io/DummyOutput.h"
#include "watermarkdata/SimpleWatermarkData.h"
#include "timeadapter/Every.h"

#include "watermark/SpectralGain.h"

#include "io/mcltproxy/MCLTInputProxy.h"
#include "io/mcltproxy/MCLTOutputProxy.h"

#include "mathutils/ssw_utils.h"

void sswencode(std::vector<int> & PNSequence, double watermarkAmplitude);
void sswdecode(std::vector<int> & PNSequence, double watermarkAmplitude, double threshold);
void TestMCLT();

void TestSSW()
{
	int SeqSize = 100;
	double watermarkAmplitude = 5.0;
	double threshold = 0.9;

	std::vector<int> PNSequence = SSWUtil::generatePNSequence(SeqSize);

	sswencode(PNSequence, watermarkAmplitude);
	sswdecode(PNSequence, watermarkAmplitude, threshold);
}

void sswdecode(std::vector<int> & PNSequence, double watermarkAmplitude, double threshold)
{
	std::cout << "Décodage..." << std::endl;

	// Instanciation des paramètres
	Parameters<double> conf;

	// Instanciation de la classe qui gère tout
	WatermarkManager<double> manager(conf);

	// Données du watermark détecté
	WatermarkData* data = new SimpleWatermarkData;

	// Instanciation du mode d'entrée et de sortie
	auto input = new FileInput<double>("out_test_ssw_mono.wav", new InputSimple<double>(conf), conf);
	auto output = new DummyOutput<double>(conf);
	// Comme c'est spectral on fait passer les entrées et sorties par un "filtre" qui va appliquer la FFT
	// Il est important que les proxy d'entrée et de sortie utilisent la même "implémentation" de FFT.
	FFT_p<double> fft_m(new FFTWManager<double>(conf)); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels((unsigned int) input->channels()); // important.
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	auto FreqRange = SSWUtil::generateFrequencyRange(PNSequence.size(), conf.bufferSize / 2 + 1);

	auto algorithm = new SSWDecode<double>(conf, PNSequence, FreqRange, watermarkAmplitude, threshold);

	// On définit tout ce petit monde. Ce sont des smart_ptr d'ou le .reset. Avantage : pas besoin de faire de delete.
	manager.data.reset(data);
	manager.input.reset(fft_i);
	manager.output.reset(fft_o);
	manager.algorithm.reset(algorithm);

	// On fait tourner l'algo
	manager.execute();

	std::cout << "Décodé" << std::endl;

	// écrire les données détectées sur la sortie standard
	//data->readSizeFromBits();
	//data->printBits();
}

void sswencode(std::vector<int> & PNSequence, double watermarkAmplitude)
{
	std::cout << "Encodage..." << std::endl;

	// Instanciation des paramètres
	Parameters<double> conf;

	// Instanciation de la classe qui gère tout
	WatermarkManager<double> manager(conf);

	//manager.timeAdapter = (TimeAdapter_p) new Every;

	// Données à watermarker
	WatermarkData* data = new SimpleWatermarkData;
	data->setSize(4);
	data->setNextBit(true);
	data->setNextBit(false);
	data->setNextBit(true);
	data->setNextBit(true);

	// Instanciation du mode d'entrée et de sortie
	auto input = new FileInput<double>("input_mono.wav", new InputSimple<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputSimple<double>(conf), conf);

	// Comme c'est spectral on fait passer les entrées et sorties par un "filtre" qui va appliquer la FFT
	// Il est important que les proxy d'entrée et de sortie utilisent la même "implémentation" de FFT.
	FFT_p<double> fft_m(new FFTWManager<double>(conf)); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels((unsigned int) input->channels()); // important.
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	// L'algo de watermarking à utiliser (ici c'est juste du gain, pas de watermark)
	//auto algorithm = new SpectralGain<double>(conf);

	auto FreqRange = SSWUtil::generateFrequencyRange(PNSequence.size(), conf.bufferSize / 2 + 1);
	auto algorithm = new SSWEncode<double>(conf, PNSequence, FreqRange, watermarkAmplitude);

	// On définit tout ce petit monde. Ce sont des smart_ptr d'ou le .reset. Avantage : pas besoin de faire de delete.
	manager.data.reset(data);
	manager.input.reset(fft_i);
	manager.output.reset(fft_o);
	manager.algorithm.reset(algorithm);

	// On fait tourner l'algo
	manager.execute();

	// On écrit dans un fichier de sortie.
	output->writeFile("out_test_ssw_mono.wav");

	std::cout << "Encodé." << std::endl;
}

/***** Test du fonctionnement de la MCLT *****/
void TestMCLT()
{
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
}
