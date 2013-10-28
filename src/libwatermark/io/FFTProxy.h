#pragma once
#include "InputManagerBase.h"
#include "OutputManagerBase.h"
#include "../fft/fftwmanager.h"

#include "copystyle/InputOLA.h"
#include "copystyle/OutputOLA.h"

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
