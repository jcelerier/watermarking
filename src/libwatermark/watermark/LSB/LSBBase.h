#pragma once

#include "../../watermark/WatermarkBase.h"

template <typename data_type>
class LSBBase: public WatermarkBase<data_type>
{
public:
	LSBBase(const Parameters<data_type>& configuration):
		WatermarkBase<data_type>(configuration)
	{
	}

	virtual ~LSBBase() = default;

	int nbBits = 10;
};
