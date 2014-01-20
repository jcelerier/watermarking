#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"

#include "properties/Amplitude.h"

template <typename data_type>
class Amplify : public BenchmarkBase<data_type>, public AmplitudeProperty
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		Amplify(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Amplify", configuration)
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
					return x * _amplitude;
				});
			}
		}

		void setGain(double g)
		{
			_amplitude = g;
		}

	private:
		double _amplitude = 0.5;
};
