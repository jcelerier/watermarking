#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputOLA.h"

template<class T>
class FFTInputProxy : public FFTProxy, public InputManagerBase
{
	public:
		FFTInputProxy(FFTManager* fft, const Parameters& cfg):
			FFTProxy(fft, cfg),
			InputManagerBase(new InputOLA(cfg), cfg)
		{

		}
		virtual IData* getNextBuffer()
		{
			if(_pos < _baseData.size())
			{
				_copy->copy(_baseData.begin(), ff->input().get().begin(), _pos, _baseData.size(), FFTProxy::conf.bufferSize);
				ff->forward();

				_pos += _copy->frameIncrement();
				return new CData<decltype(ff->spectrum())>(ff->spectrum());
			}

			_pos += _copy->frameIncrement();
			return nullptr;
		}
};


