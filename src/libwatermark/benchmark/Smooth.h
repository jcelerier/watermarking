#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

template <typename data_type>
class Smooth : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		Smooth(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Smooth", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				auto t1 = sampleData[1], t2 = sampleData[sampleData.size() - 2];
				for(auto i = 1U; i < sampleData.size() - 1; i++)
					sampleData[i] = (sampleData[i] + sampleData[i-1] + sampleData[i+1]) / 3.0;

				sampleData[0] = (sampleData[0] + t1) / 2.0;
				sampleData[sampleData.size() - 1] =  (sampleData[sampleData.size() - 1] + t2) / 2.0;
			}
		}
};
