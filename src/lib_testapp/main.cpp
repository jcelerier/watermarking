#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "watermark/GainTest.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "fft/fftwmanager.h"
#include "watermark/SpectralGain.h"
#include "io/BufferInput.h"
#include "io/BufferOutput.h"
/***
 *
 * Ce fichier montre quelques tests cool.
 * Pour voir comment écrire les algo de watermark : se référer à GainTest.h pour un algo temporel
 * et SpectralGain.h pour un algo spectral.
 *
 * Important : pour l'instant il n'y a pas de gestion des "données" à encoder (genre le message qu'on veut mettre dans la watermark).
 * Il faudra surcharger l'opérateur de foncteur de WatermarkBase et lui rajouter ça en paramètre.
 */
//A faire: gestion de la stéréo
//TODO multicanal
//TODO module de sortie audio
//TODO module de sortie gnuplot
// Test : réduction spectrale de gain (on divise chaque bande du spectre)

void SpectralTest()
{
	// Instanciation des paramètres
	Parameters<double> conf;

	// Instanciation de la classe qui gère tout
	auto manager = new WatermarkManager<double>(conf);

	// Instanciation du mode d'entrée et de sortie
	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	// Comme c'est spectral on fait passer les entrées et sorties par un "filtre" qui va appliquer la FFT
	// Il est important que les proxy d'entrée et de sortie utilisent la même "implémentation" de FFT.
	auto fft_m = new FFTWManager<double>(conf); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels(1); // important.
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	// L'algo de watermarking à utiliser (ici c'est juste du gain, pas de watermark)
	auto algorithm = new SpectralGain<double>(conf);

	// On définit tout ce petit monde. Ce sont des smart_ptr d'ou le .reset. Avantage : pas besoin de faire de delete.
	manager->_input.reset(fft_i);
	manager->_output.reset(fft_o);
	manager->_watermark.reset(algorithm);

	// On fait tourner l'algo
	manager->execute();

	// On écrit dans un fichier de sortie.
	output->writeFile("out_test_spec.wav");
}

// Test : réduction temporelle de gain ( on divise chaque sample).
void TemporalTest()
{
	Parameters<double> conf;
	auto manager = new WatermarkManager<double>(conf);

	// Ici on n'utilise pas le proxy FFT
	auto input = new FileInput<double>("input_mono.wav", conf);
	auto output = new FileOutput<double>(conf);

	auto algorithm = new GainTest<double>(conf);

	manager->_input.reset(input);
	manager->_output.reset(output);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeFile("out_test_temp.wav");
}

// Test : la même chose avec des shorts au lieu de double. Cela permet
// l'accès à l'information des bits et permet le codage par LSB. Poke qmidy.
// (rappel : format des samples standard dans un fichier wav, cd audio, etc... = 16 bit signé (i.e. short))
void TemporalTestShorts()
{
	Parameters<short> conf;
	auto manager = new WatermarkManager<short>(conf);

	auto input = new FileInput<short>("input_mono.wav", conf);
	auto output = new FileOutput<short>(conf);

	auto algorithm = new GainTest<short>(conf);

	manager->_input.reset(input);
	manager->_output.reset(output);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeFile("out_test_shorts.wav");
}

void TestFFTWManager()
{
	// Generer une sine
	Parameters<double> conf;
	auto fft_m = new FFTWManager<double>(conf);
	fft_m->setChannels(1);
	for(auto i = 0U; i < conf.bufferSize; ++i)
		fft_m->input()[0][i] = sin(440.0 * (2.0 * 3.1415) * i / (double) conf.samplingRate);

	fft_m->forward();
	fft_m->backward();

	for(auto i = 0U; i < conf.bufferSize; ++i)
		std::cerr << fft_m->input()[0][i] << "\t\t" << fft_m->output()[0][i] / (double) conf.bufferSize << std::endl;
}
/*
void BufferTest()
{
	Parameters<double> conf;

	auto n = 4096U;
	short* in_test = new short[n];

	for(auto i = 0U; i < n; ++i)
		in_test[i] = 16384 * sin(220.0 * (2.0 * 3.1415) * i / (double) conf.samplingRate);

	auto format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	auto channels = 1;
	SndfileHandle outfile("out_test_buffer.wav", SFM_WRITE, format, channels, (int) conf.samplingRate);

	auto input = new BufferInput<double>(conf);
	auto output = new BufferOutput<double>(conf);

	input->readBuffer(in_test, n);

	auto algorithm = new GainTest<double>(conf);

	auto manager = new WatermarkManager<double>(conf);
	manager->_input.reset(input);
	manager->_output.reset(output);
	manager->_watermark.reset(algorithm);

	manager->execute();

	output->writeOutBuffer(in_test);

	outfile.write(in_test,
				  n);
}
*/
int main()
{
	TemporalTest();
	return 0;
}

