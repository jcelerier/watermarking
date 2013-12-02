#pragma once
#include "../../fft/FFTManager.h"

template <typename data_type>
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

		virtual ~FFTProxy() { std::cerr << "coucou\n"; }
};
