#pragma once
#include <algorithm>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class BufferOutput : public OutputManagerBase
{
	public:
		void writeOutBuffer(short * address)
		{
			std::transform(_baseData.begin(), _baseData.end(), address, MathUtil::DoubleToShort);
		}
};
