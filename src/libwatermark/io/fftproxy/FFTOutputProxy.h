#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/OutputSimple.h"
#include "../../mathutils/math_util.h"
class FFTOutputProxy : public FFTProxy, public OutputManagerBase
{
	public:
		FFTOutputProxy(OutputManagerBase* output, FFTManager* fft, const Parameters& cfg):
			FFTProxy(fft, cfg),
			OutputManagerBase(new OutputSimple(cfg), cfg),
			_outputImpl(output)
		{

		}

		virtual void writeNextBuffer(IData*)
		{
			// 0. We use the buffer already in the FFT.

			// 1. Perform reverse FFT
			fft->backward();

			// 2. Copy the content of the FFT output into inner buffer
			_outputImpl->_baseData.resize(_outputImpl->_baseData.size() + FFTProxy::conf.bufferSize);

			_copy->copy(fft->output().begin(),
						_outputImpl->_baseData.begin(),
						_pos,
						FFTProxy::conf.bufferSize,
						_outputImpl->_baseData.size());

			// 3. NORMALIZE THE SHIT OUT OF IT
			std::transform(_outputImpl->_baseData.begin() + _pos,
						   _outputImpl->_baseData.begin() + _pos + FFTProxy::conf.bufferSize,
						   _outputImpl->_baseData.begin() + _pos,
						   [this] (double x) { return x * fft->normalizationFactor(); });

			_pos += _copy->frameIncrement();
		}

		Output_p _outputImpl = nullptr;
};


