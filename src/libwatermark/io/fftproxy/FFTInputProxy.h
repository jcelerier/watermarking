#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputSimple.h"
#include "../copystyle/InputOLA.h"
#include <utility>
// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::pos;
		using InputManagerBase<data_type>::copyHandler;
		using FFTProxy<data_type>::_fft;

	private:
		Input_p _inputImpl = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input,
					  FFTManager<data_type>* fftmanager,
					  const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(new InputOLA<data_type>(cfg), cfg),
			_inputImpl(input)
		{
		}

		virtual IData* getNextBuffer() final override
		{
			auto& vec = dynamic_cast<InputManagerBase<data_type>*>(_inputImpl.get())->v();

			auto channels = vec.size();
			auto frames = vec[0].size();

			if(pos() < frames)
			{
				// 1. copier la partie du buffer à traiter
				for(auto i = 0U; i < channels; ++i)
				{
					copyHandler->copy(vec[i].begin(),
									_fft->input()[i].begin(),
									pos(),
									frames);
				}


				// 2. Effectuer la FFT
				_fft->forward();

				// 3. Empaqueter le spectre donné par la FFT
				auto buffer = new CData<typename FFTProxy<data_type>::complex_type>;

				buffer->_data = std::move(_fft->spectrum());

				pos() += copyHandler->frameIncrement();
				return buffer;
			}

			return nullptr;
		}
};
