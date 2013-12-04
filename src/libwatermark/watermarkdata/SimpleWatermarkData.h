#pragma once
#include "WatermarkData.h"


class SimpleWatermarkData : public WatermarkData
{
	public:
		virtual bool nextBit() final override
		{
			return bits[_position++];
		}

		virtual bool isComplete() final override
		{
			return _position >= _size;
		}
};
