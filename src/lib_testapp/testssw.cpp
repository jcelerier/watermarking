#include "WatermarkManager.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "watermark/GainTest.h"
#include "watermark/LSBEncode.h"
#include "io/fftproxy/FFTInputProxy.h"
#include "io/fftproxy/FFTOutputProxy.h"
#include "fft/FFTWManager.h"
#include "watermark/SpectralGain.h"
#include "io/BufferInput.h"
#include "io/BufferOutput.h"
#include "io/SilenceInput.h"


void sswencode();
void sswdecode();

void TestSSW()
{
	sswencode();
	sswdecode();
}

void sswdecode(){//todo
}
void sswencode()
{
	// Instanciation des paramètres
	Parameters<double> conf;

	// Instanciation de la classe qui gère tout
	WatermarkManager<double> manager(conf);

	// Données à watermarker
	WatermarkData* data = new WatermarkData;
	data->setSize(4);
	data->bits.push_back(true);
	data->bits.push_back(false);
	data->bits.push_back(true);
	data->bits.push_back(true);

	// Instanciation du mode d'entrée et de sortie
	auto input = new FileInput<double>("input_mono.wav", new InputOLA<double>(conf), conf);
	auto output = new FileOutput<double>(new OutputOLA<double>(conf), conf);

	// Comme c'est spectral on fait passer les entrées et sorties par un "filtre" qui va appliquer la FFT
	// Il est important que les proxy d'entrée et de sortie utilisent la même "implémentation" de FFT.
	FFT_p<double> fft_m(new FFTWManager<double>(conf)); // -> Utilise FFTW. On peut facilement écrire des wrapper pour d'autres libs de FFT.
	fft_m->setChannels((unsigned int) input->channels()); // important.
	auto fft_i = new FFTInputProxy<double>(input, fft_m, conf);
	auto fft_o = new FFTOutputProxy<double>(output, fft_m, conf);

	// L'algo de watermarking à utiliser (ici c'est juste du gain, pas de watermark)
	auto algorithm = new SpectralGain<double>(conf);

	// On définit tout ce petit monde. Ce sont des smart_ptr d'ou le .reset. Avantage : pas besoin de faire de delete.
	manager.data.reset(data);
	manager.input.reset(fft_i);
	manager.output.reset(fft_o);
	manager.algorithm.reset(algorithm);

	// On fait tourner l'algo
	manager.execute();

	// On écrit dans un fichier de sortie.
	output->writeFile("out_test_ssw_mono.wav");
}
