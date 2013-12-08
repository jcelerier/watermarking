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
			BenchmarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			for(auto& sampleData : channelsData)
			{
				std::vector<data_type> y(sampleData.size());
				for (int i = 0; i < sampleData.size(); i++ )
				{
					y[i] = 0;                       // set to zero before sum
					for (int j = 0; j < h.size(); j++ )
					{
						if(i - j >= 0)
							y[i] += sampleData[i - j] * h[j];    // convolve: multiply and accumulate
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
