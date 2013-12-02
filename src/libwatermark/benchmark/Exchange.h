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
			BenchmarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			for(auto& sampleData : channelsData)
			{
				for(auto i = 0U; i < sampleData.size(); i += 2)
				{
					std::swap(sampleData[i], sampleData[i+1]);
				}
			}
		}


		void setAmpli(double amp)
		{
			_amplitude = amp;
		}

	private:
		unsigned int _frequence = 50;
		data_type _amplitude = 1.0;
		std::default_random_engine rng = std::default_random_engine(std::random_device{}());
		std::uniform_real_distribution<double> dist = std::uniform_real_distribution<double>(-1, 1);

};
