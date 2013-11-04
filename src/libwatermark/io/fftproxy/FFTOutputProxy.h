#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/OutputSimple.h"
#include "../copystyle/OutputOLA.h"
#include "../../mathutils/math_util.h"

template <typename data_type>
class FFTOutputProxy : public FFTProxy<data_type>, public OutputManagerBase<data_type>
{
		using OutputManagerBase<data_type>::pos;
		using OutputManagerBase<data_type>::copyHandler;
		using FFTProxy<data_type>::_fft;
	private:
		Output_p _outputImpl = nullptr;

	public:
		FFTOutputProxy(OutputManagerBase<data_type>* output, FFTManager<data_type>* fft, const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fft, cfg),
			OutputManagerBase<data_type>(new OutputOLA<data_type>(cfg), cfg),
			_outputImpl(output)
		{
		}

		virtual void writeNextBuffer(IData* buf) final override
		{
			auto& buffer = dynamic_cast<CData<typename FFTProxy<data_type>::complex_type>*>(buf)->_data;
			auto& out_vec = dynamic_cast<OutputManagerBase<data_type>*>(_outputImpl.get())->v();

			auto channels = buffer.size();
			if(out_vec.size() != channels)
			{
				out_vec.resize(channels);
			}

			// 0. We put our buffer back in the FFT.
			_fft->spectrum() = std::move(buffer);

			// 1. Perform reverse FFT
			_fft->backward();

			// 2. Copy the content of the FFT output into inner buffer
			for(auto i = 0U; i < channels; ++i) // Pour chaque canal
			{
				out_vec[i].resize(out_vec[i].size() + copyHandler->frameIncrement());

				copyHandler->copy(_fft->output()[i].begin(),
								  out_vec[i].begin(),
								  pos(),
								  out_vec[i].size());

				// 3. NORMALIZE THE SHIT OUT OF IT
				std::transform(out_vec[i].begin() + pos(),
							   out_vec[i].begin() + pos() + copyHandler->frameIncrement(),
							   out_vec[i].begin() + pos(),
							   [this] (data_type x) { return x * _fft->normalizationFactor(); }
				);
			}
			pos() += copyHandler->frameIncrement();
		}


};


