#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputOLA.h"
#include "window/HannWindow.h"
#include "window/BartlettWindow.h"
#include "window/BlackmanWindow.h"
#include "window/RectWindow.h"

template <typename data_type>
/**
 * @brief The FFTInputProxy class
 *
 * Réalise une STFT (short time fourier transform)
 */
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

		using FFTProxy<data_type>::fft;
		using complex_type = typename Parameters<data_type>::complex_type;

	private:
		Input_p<data_type> inputImpl = nullptr;
		Window_p<data_type> window = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input,
					  FFT_p<data_type> fftmanager,
					  Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(nullptr, cfg),
			inputImpl(input),
			window(new HannWindow<data_type>(cfg))
		{
		}

		virtual ~FFTInputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			// 1. On get le buffer.
			Audio_p tmp = inputImpl->getNextBuffer();
			if(tmp == nullptr) return tmp;
			auto& inbuff = static_cast<CData<data_type>*>(tmp.get())->_data;

			// 2. On fenêtre
			for(auto& channel : inbuff)
				window->apply(channel, channel.size());

			// 3. On copie dans le buffer de la fft
			std::copy(inbuff.begin(), inbuff.end(), fft->input().begin());

			// 3. fft
			fft->forward();

			// 4. On crée un autre buffer pour le spectre
			auto outbuff = new CData<complex_type>;

			// 5. On move fft.spectrum
			outbuff->_data = std::move(fft->spectrum());

			// 6. On retourne
			return Audio_p(outbuff);
		}
};
