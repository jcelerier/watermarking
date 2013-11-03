#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputOLA.h"

// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
	private:
		Input_p _inputImpl = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input, FFTManager<data_type>* fftmanager, const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(new InputOLA<data_type>(cfg), cfg),
			_inputImpl(input)
		{

		}

		virtual IData* getNextBuffer() final override
		{
			auto input_real = dynamic_cast<InputManagerBase<data_type>*>(_inputImpl.get());
			if(this->pos() < input_real->_baseData.size())
			{
				// 1. copier la partie du fichier à traiter
				this->_copy->copy(input_real->_baseData.begin(),
							this->_fft->input().begin(),
							this->pos(),
							input_real->_baseData.size(),
							FFTProxy<data_type>::conf.bufferSize);

				// 2. Effectuer la FFT
				this->_fft->forward();

				this->pos() += this->_copy->frameIncrement();

				// 3. Empaqueter le spectre donné par la FFT
				auto b = new CData<typename FFTProxy<data_type>::complex_type>;

				b->_data = this->_fft->moveSpectrum();
				return b;
			}

			return nullptr;
		}


};


