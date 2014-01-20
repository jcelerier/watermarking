#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"
#include "../mathutils/math_util.h"

#include "properties/Amplitude.h"

template <typename data_type>
class AddWhiteNoise : public BenchmarkBase<data_type>, public AmplitudeProperty
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		AddWhiteNoise(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("AddWhiteNoise", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				MathUtil::apply(sampleData,
					  [this] (data_type x)
				{
					return x + _amplitude * dist(rng);
				});
			}
		}


	private:
		std::default_random_engine rng = std::default_random_engine(std::random_device{}());
		std::uniform_real_distribution<double> dist = std::uniform_real_distribution<double>(-1, 1);
};
