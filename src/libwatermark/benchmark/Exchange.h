#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

template <typename data_type>
class Exchange : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		Exchange(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Exchange", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				for(auto i = 0U; i < sampleData.size(); i += 2)
				{
					std::swap(sampleData[i], sampleData[i+1]);
				}
			}
		}
};
