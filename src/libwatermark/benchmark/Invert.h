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
			BenchmarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			for(auto& sampleData : channelsData)
			{
				std::transform(sampleData.begin(),
							   sampleData.end(),
							   sampleData.begin(),
							   [] (data_type x)
				{
					return conf.normFactor() - x;
				});
			}
		}
};
