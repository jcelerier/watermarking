#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputOLA.h"
#include "window/HannWindow.h"
#include "window/RectWindow.h"
// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		using InputManagerBase<data_type>::pos;
		using InputManagerBase<data_type>::copyHandler;
		using FFTProxy<data_type>::fft;

	private:
		Input_p<data_type> inputImpl = nullptr;
		Window_p<data_type> window = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input,
					  FFT_p<data_type> fftmanager,
					  const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(new InputOLA<data_type>(cfg), cfg),
			inputImpl(input),
			window(new RectWindow<data_type>(cfg))
		{
		}

		virtual std::vector<std::vector<data_type> >& v()
		{
			return inputImpl->v();
		}

		virtual ~FFTInputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			if(pos() < frames())
			{
				auto buffer = new CData<typename FFTProxy<data_type>::complex_type>;

				// Pour chaque canal
				for(auto i = 0U; i < channels(); ++i)
				{
					// 1. copier la partie du buffer à traiter
					copyHandler->copy(v()[i].begin(),
									  fft->input()[i].begin(),
									  pos(),
									  frames());

					// 2. Appliquer fenêtrage
					window->apply(fft->input()[i], copyHandler->frameIncrement());
				}

				// 3. Effectuer la FFT
				fft->forward();

				// 4. Empaqueter le spectre donné par la FFT
				buffer->_data = std::move(fft->spectrum());

				pos() += copyHandler->frameIncrement();
				return Audio_p(buffer);
			}

			return Audio_p(nullptr);
		}

};
