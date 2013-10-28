#include <cmath>

#include "bypass.h"
#include "../mathutils/math_util.h"
#include "../watermark_manager.h"

BypassWatermark::BypassWatermark(const Parameters& configuration):
	SpectralWatermarkBase(configuration)
{
}

SpectralWatermarkBase *BypassWatermark::clone()
{
	return new BypassWatermark(*this);
}

void BypassWatermark::operator()(std::complex<double> * const input_spectrum)
{
	for (auto i = 0U; i < conf.bufferSize; ++i)
	{
		// Do stuff
	}
}

void BypassWatermark::onFFTSizeUpdate()
{

}

void BypassWatermark::onDataUpdate()
{

}
