#pragma once
#include "../InputManagerBase.h"
#include "../OutputManagerBase.h"
#include "../../fft/fftwmanager.h"




class FFTProxy
{
	public:
		FFTProxy(FFTManager* fft, const Parameters& cfg):
			ff(fft),
			conf(cfg)
		{

		}

	protected:
		FFTManager* ff;

		const Parameters& conf;

};
