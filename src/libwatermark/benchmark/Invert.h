#pragma once
#include <algorithm>

#include "BenchmarkBase.h"

template <typename data_type>
class Invert : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		Invert(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("FFTInvert", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				MathUtil::apply(sampleData,
					  [&] (data_type x)
				{
					return -x;
				});
			}
		}
};
