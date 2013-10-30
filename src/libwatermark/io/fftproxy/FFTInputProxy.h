#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputSimple.h"

// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
	private:
		Input_p _inputImpl = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input, FFTManager<data_type>* fftmanager, const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(new InputSimple<data_type>(cfg), cfg),
			_inputImpl(input)
		{

		}

		virtual IData* getNextBuffer() const final override
		{
			auto input_real = dynamic_cast<InputManagerBase<data_type>*>(_inputImpl.get());
			if(InputManagerBase<data_type>::_pos < input_real->_baseData.size())
			{
				// 1. copier la partie du fichier à traiter
				InputManagerBase<data_type>::_copy->copy(input_real->_baseData.begin(),
							FFTProxy<data_type>::_fft->input().begin(),
							InputManagerBase<data_type>::_pos,
							input_real->_baseData.size(),
							FFTProxy<data_type>::conf.bufferSize);

				// 2. Effectuer la FFT
				FFTProxy<data_type>::_fft->forward();

				InputManagerBase<data_type>::_pos += InputManagerBase<data_type>::_copy->frameIncrement();

				// 3. Empaqueter le spectre donné par la FFT
				auto b = new CData<typename FFTProxy<data_type>::complex_type>;
				b->_data = FFTProxy<data_type>::_fft->spectrum();
				return b;
			}

			return nullptr;
		}


};


