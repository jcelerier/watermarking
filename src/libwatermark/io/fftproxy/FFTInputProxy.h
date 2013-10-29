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
				auto b = new CData<std::complex<data_type>>;
				_copy->copy(_baseData.begin(), ff->input().get().begin(), _pos, _baseData.size(), FFTProxy::conf.bufferSize);

				return b;
			}

			_pos += _copy->frameIncrement();
			return nullptr;
		}
};


