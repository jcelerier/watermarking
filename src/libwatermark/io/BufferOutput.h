#pragma once
#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"
#include <algorithm>

class BufferOutput : public OutputManagerBase
{
	public:
		void writeOutBuffer(short * address)
		{
			std::transform(_baseData.begin(), _baseData.end(), address, MathUtil::DoubleToShort);
		}
};
