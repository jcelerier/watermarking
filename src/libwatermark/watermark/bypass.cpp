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

void BypassWatermark::operator()(IData* const input_spectrum)
{
	auto b = dynamic_cast<CData<std::complex<data_type>>*>(data);;
	for (auto i = 0U; i < b->_data.size(); ++i)
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
