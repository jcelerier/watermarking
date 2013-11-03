#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputOLA.h"

// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
		using IOManagerBase<data_type>::pos;
		using FFTProxy<data_type>::_fft;

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
			auto input_mgr = dynamic_cast<InputManagerBase<data_type>*>(_inputImpl.get());

			if(this->pos() < input_mgr->_baseData[0].size())
			{
				// 1. copier la partie du buffer à traiter
				for(auto i = 0U; i < this->v().size(); ++i)
				{
					this->_copy->copy(input_mgr->v()[i].begin(),
									_fft->input()[i].begin(),
									pos(),
									input_mgr->v()[i].size(),
									FFTProxy<data_type>::conf.bufferSize);
				}


				// 2. Effectuer la FFT
				_fft->forward();

				pos() += this->_copy->frameIncrement();

				// 3. Empaqueter le spectre donné par la FFT
				auto b = new CData<typename FFTProxy<data_type>::complex_type>;

				b->_data = _fft->moveSpectrum();
				return b;
			}

			return nullptr;
		}


};


