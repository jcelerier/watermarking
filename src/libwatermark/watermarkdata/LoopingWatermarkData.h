#pragma once
#include "WatermarkData.h"


class LoopingWatermarkData : public WatermarkData
{
	public:
		virtual bool nextBit() final override
		{
			if(_position == bits.size()) _position = 0;
			return bits[_position++];
		}

		virtual bool isComplete() final override
		{
			return false;
		}
};
