#pragma once
#include <algorithm>

#include "watermarkbase.h"
template <typename data_type>
class GainTest : public WatermarkBase<data_type>
{
	public:
		GainTest(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{

		}

		virtual GainTest* clone()
		{
			return new GainTest<data_type>(*this);
		}

		virtual void operator()(IData* const data)
		{
			CData<data_type>* b = dynamic_cast<CData<data_type>*>(data);

			std::transform(b->_data.begin(),
						   b->_data.end(),
						   b->_data.begin(),
						   [&] (double x) { return x * _gain; });

		}

		virtual void onDataUpdate()
		{

		}

	private:
		double _gain = 0.1;
};
