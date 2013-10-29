#pragma once
#include "../InputManagerBase.h"
#include "../OutputManagerBase.h"
#include "../../fft/fftwmanager.h"




class FFTProxy
{

	public:
		using complex_type = Parameters::complex_type;
		FFTProxy(FFTManager* fftmanager, const Parameters& cfg):
			fft(fftmanager),
			conf(cfg)
		{

		}

		virtual ~FFTProxy() = default;
	protected:
		std::shared_ptr<FFTManager> fft;

		const Parameters& conf;

};
