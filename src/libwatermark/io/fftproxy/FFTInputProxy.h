#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputSimple.h"
#include "../copystyle/InputOLA.h"

// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		using InputManagerBase<data_type>::pos;
		using InputManagerBase<data_type>::copyHandler;
		using FFTProxy<data_type>::_fft;

	private:
		Input_p<data_type> _inputImpl = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input,
					  FFT_p<data_type> fftmanager,
					  const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(new InputOLA<data_type>(cfg), cfg),
			_inputImpl(input)
		{
		}

		virtual std::vector<std::vector<data_type> >& v()
		{
			return _inputImpl->v();
		}

		virtual ~FFTInputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			if(pos() < frames())
			{
				auto buffer = new CData<typename FFTProxy<data_type>::complex_type>;
				// 1. copier la partie du buffer à traiter
				for(auto i = 0U; i < channels(); ++i)
				{
					copyHandler->copy(v()[i].begin(),
									  _fft->input()[i].begin(),
									  pos(),
									  frames());
				}

				// 2. Effectuer la FFT
				_fft->forward();

				// 3. Empaqueter le spectre donné par la FFT
				buffer->_data = std::move(_fft->spectrum());

				pos() += copyHandler->frameIncrement();
				return Audio_p(buffer);
			}

			return Audio_p(nullptr);
		}

};
