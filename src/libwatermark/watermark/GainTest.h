#pragma once
#include "watermarkbase.h"
#include <algorithm>

class GainTest : public WatermarkBase
{
	public:
		GainTest();
		virtual GainTest* clone()
		{
			return new GainTest(*this);
		}

		virtual void operator()(IData* const data)
		{
			CData<data_type>* b = dynamic_cast<CData<data_type>*>(data);;
			std::transform(b->_data.begin(), b->_data.end(), b->_data.begin(), [&] (double x) { return x * _gain; });
		}

		virtual void onDataUpdate()
		{

		}

	private:
		double _gain = 0.1;
};
