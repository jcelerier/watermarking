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
			_input(input)
		{

		}

		virtual IData* getNextBuffer()
		{
			if(_pos < _input->_baseData.size())
			{
				// 1. copier la partie du fichier à traiter
				_copy->copy(_input->_baseData.begin(), fft->input().begin(), _pos, _input->_baseData.size(), FFTProxy::conf.bufferSize);

				std::cerr << "Dans 0INPUT << " << fft->spectrum()[0] << std::endl;
				// 2. Effectuer la FFT
				fft->forward();
				std::cerr << "Dans INPUT << " << fft->spectrum()[0] << std::endl;
				_pos += _copy->frameIncrement();

				// 3. Empaqueter le spectre donné par la FFT
				auto b = new CData<complex_type>;
				b->_data.resize(fft->spectrumSize());
				std::copy(fft->spectrum().begin(), fft->spectrum().end(), b->_data.begin());
				return b;
			}

			return nullptr;
		}

		Input_p _input = nullptr;
};


