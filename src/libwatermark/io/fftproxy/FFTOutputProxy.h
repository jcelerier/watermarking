#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/OutputSimple.h"
#include "../copystyle/OutputOLA.h"
#include "../../mathutils/math_util.h"

template <typename data_type>
class FFTOutputProxy : public FFTProxy<data_type>, public OutputManagerBase<data_type>
{
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;
		using OutputManagerBase<data_type>::pos;
		using OutputManagerBase<data_type>::copyHandler;
		using FFTProxy<data_type>::fft;
	private:
		Output_p<data_type> outputImpl = nullptr;

	public:
		FFTOutputProxy(OutputManagerBase<data_type>* output, FFT_p<data_type> fft_impl, const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fft_impl, cfg),
			OutputManagerBase<data_type>(new OutputOLA<data_type>(cfg), cfg),
			outputImpl(output)
		{
		}

		virtual ~FFTOutputProxy() = default;

		virtual std::vector<std::vector<data_type> >& v()
		{
			return outputImpl->v();
		}

		virtual void writeNextBuffer(Audio_p& buf) final override
		{
			auto& buffer = static_cast<CData<typename FFTProxy<data_type>::complex_type>*>(buf.get())->_data;

			if(channels() != buffer.size())
			{
				v().resize(buffer.size());
			}

			// 0. We put our buffer back in the FFT.
			fft->spectrum() = std::move(buffer);

			// 1. Perform reverse FFT
			fft->backward();

			// 2. Copy the content of the FFT output into inner buffer
			for(auto i = 0U; i < channels(); ++i) // Pour chaque canal
			{
				v()[i].resize(v()[i].size() + copyHandler->frameIncrement());

				copyHandler->copy(fft->output()[i].begin(),
								  v()[i].begin(),
								  pos(),
								  v()[i].size());

				// 3. NORMALIZE THE SHIT OUT OF IT
				std::transform(v()[i].begin() + pos(),
							   v()[i].begin() + pos() + copyHandler->frameIncrement(),
							   v()[i].begin() + pos(),
							   [this] (data_type x) { return x * fft->normalizationFactor(); }
				);
			}
			pos() += copyHandler->frameIncrement();
		}
};


