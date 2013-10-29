#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputSimple.h"

class FFTInputProxy : public FFTProxy, public InputManagerBase
{
	public:
		FFTInputProxy(InputManagerBase* input, FFTManager* fftmanager, const Parameters& cfg):
			FFTProxy(fftmanager, cfg),
			InputManagerBase(new InputSimple(cfg), cfg),
			_inputImpl(input)
		{

		}

		virtual IData* getNextBuffer()
		{
			if(_pos < _inputImpl->_baseData.size())
			{
				// 1. copier la partie du fichier à traiter
				_copy->copy(_inputImpl->_baseData.begin(), fft->input().begin(), _pos, _inputImpl->_baseData.size(), FFTProxy::conf.bufferSize);

				// 2. Effectuer la FFT
				fft->forward();

				_pos += _copy->frameIncrement();

				// 3. Empaqueter le spectre donné par la FFT
				auto b = new CData<complex_type>;
				b->_data = fft->spectrum();
				return b;
			}

			return nullptr;
		}

		Input_p _inputImpl = nullptr;
};


