#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/OutputSimple.h"
#include "../../mathutils/math_util.h"

template <typename data_type>
class FFTOutputProxy : public FFTProxy<data_type>, public OutputManagerBase<data_type>
{
	private:
		Output_p _outputImpl = nullptr;

	public:
		FFTOutputProxy(OutputManagerBase<data_type>* output, FFTManager<data_type>* fft, const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fft, cfg),
			OutputManagerBase<data_type>(new OutputSimple<data_type>(cfg), cfg),
			_outputImpl(output)
		{

		}

		virtual void writeNextBuffer(IData* buf) final override
		{
			auto output_real = dynamic_cast<OutputManagerBase<data_type>*>(_outputImpl.get());
			// 0. We use the buffer already in the FFT.
			auto b = dynamic_cast<CData<typename FFTProxy<data_type>::complex_type>*>(buf);
			FFTProxy<data_type>::_fft->spectrum() = b->_data;

			// 1. Perform reverse FFT
			FFTProxy<data_type>::_fft->backward();

			// 2. Copy the content of the FFT output into inner buffer
			output_real->_baseData.resize(output_real->_baseData.size() + FFTProxy<data_type>::conf.bufferSize);

			OutputManagerBase<data_type>::_copy->copy(FFTProxy<data_type>::_fft->output().begin(),
						output_real->_baseData.begin(),
						OutputManagerBase<data_type>::_pos,
						FFTProxy<data_type>::conf.bufferSize,
						output_real->_baseData.size());

			// 3. NORMALIZE THE SHIT OUT OF IT
			std::transform(output_real->_baseData.begin() + OutputManagerBase<data_type>::_pos,
						   output_real->_baseData.begin() + OutputManagerBase<data_type>::_pos + FFTProxy<data_type>::conf.bufferSize,
						   output_real->_baseData.begin() + OutputManagerBase<data_type>::_pos,
						   [&] (data_type x) { return x * FFTProxy<data_type>::_fft->normalizationFactor(); }
						  );

			OutputManagerBase<data_type>::_pos += OutputManagerBase<data_type>::_copy->frameIncrement();
		}


};


