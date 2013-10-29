#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/OutputOLA.h"
class FFTOutputProxy : public FFTProxy, public OutputManagerBase
{
	public:
		FFTOutputProxy(FFTManager* fft, const Parameters& cfg):
			ff(fft),
			conf(cfg)
		{

		}

		virtual void writeNextBuffer(IData* buffer)
		{

		}
};


