#pragma once

#include "../InputManagerBase.h"
#include "../OutputManagerBase.h"
#include "../../fft/FFTManager.h"

template <typename data_type>
class FFTProxy
{
	protected:
		const Parameters<data_type>& conf;

	public:
		std::shared_ptr<FFTManager<data_type>> fft;
		using complex_type = typename Parameters<data_type>::complex_type;

		FFTProxy(FFT_p<data_type> fftmanager, const Parameters<data_type>& cfg):
			conf(cfg),
			fft(fftmanager)
		{
		}

		virtual ~FFTProxy() = default;
};
