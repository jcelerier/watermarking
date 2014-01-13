#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"
#include "mathutils/math_util.h"

template <typename data_type>
class Convolution : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		Convolution(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Convolution", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				std::vector<data_type> y(sampleData.size());
				for (auto i = 0U; i < sampleData.size(); i++)
				{
					y[i] = 0;
					for (auto j = 0U; j < h.size() && i >= j; j++ )
					{
						y[i] += sampleData[i - j] * h[j];
					}
				}
				sampleData = y;
			}
		}

		void setFilter(std::vector<data_type>& t)
		{
			h = t;
		}

	private:
		std::vector<data_type> h = {0.0140, 0.0299, 0.0719, 0.1246, 0.1677,
									0.1842, 0.1677, 0.1246, 0.0719, 0.0299, 0.0140};
};
