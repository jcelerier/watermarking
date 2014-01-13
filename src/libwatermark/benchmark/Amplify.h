#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"
#include "mathutils/math_util.h"

template <typename data_type>
class Amplify : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		Amplify(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>(configuration)
		{
			this->_name = "Amplify";
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			for(auto& sampleData : channelsData)
			{
				MathUtil::apply(sampleData,
					  [&] (data_type x)
				{
					return x * _gain;
				});
			}
		}

		void setGain(double g)
		{
			_gain = g;
		}

	private:
		double _gain = 0.5;
};
