#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/OutputOLA.h"
#include "../../mathutils/math_util.h"

template <typename data_type>
class FFTOutputProxy : public FFTProxy<data_type>, public OutputManagerBase<data_type>
{
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
			auto output_real = dynamic_cast<OutputManagerBase<data_type>*>(_outputImpl.get());

			//Faire un vector de plans dans le fftwmanager
			// 0. We use the buffer already in the FFT.
			this->_fft->setSpectrum(std::move(dynamic_cast<CData<typename FFTProxy<data_type>::complex_type>*>(buf)->_data));

			// 1. Perform reverse FFT
			this->_fft->backward();

			// 2. Copy the content of the FFT output into inner buffer
			output_real->_baseData.resize(output_real->_baseData.size() + this->_copy->frameIncrement());

			this->_copy->copy(this->_fft->output().begin(),
						output_real->_baseData.begin(),
						this->pos(),
						FFTProxy<data_type>::conf.bufferSize,
						output_real->_baseData.size());

			// 3. NORMALIZE THE SHIT OUT OF IT
			std::transform(output_real->_baseData.begin() + this->pos(),
						   output_real->_baseData.begin() + this->pos() + this->_copy->frameIncrement(),
						   output_real->_baseData.begin() + this->pos(),
						   [&] (data_type x) { return x * this->_fft->normalizationFactor(); }
						  );

			this->pos() += this->_copy->frameIncrement();
		}


};


