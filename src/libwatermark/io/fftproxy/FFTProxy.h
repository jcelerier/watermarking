#pragma once
#include "../../fft/FFTManager.h"

template <typename data_type>
/**
 * @brief The FFTProxy class
 *
 * Classe de base pour la transformée de fourier.
 * Comme FFTW utilise des plans définis à la construction,
 * on a besoin que ce soit le même FFTManager qui serve
 * à l'initialisation de la fft et de la fft inverse.
 */
class FFTProxy
{
	protected:
		const Parameters<data_type>& conf;

	public:
		FFT_p<data_type> fft;
		using complex_type = typename Parameters<data_type>::complex_type;

		FFTProxy(FFT_p<data_type> fftmanager, Parameters<data_type>& cfg):
			conf(cfg),
			fft(fftmanager)
		{
		}

		virtual ~FFTProxy() = default;
};
