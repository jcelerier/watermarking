#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"
#include "../mathutils/math_util.h"
#include "properties/Threshold.h"
template <typename data_type>
class ZeroCross : public BenchmarkBase<data_type>, public ThresholdProperty
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
		ZeroCross(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("ZeroCross", configuration)
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
					return x > _thr ? x : 0;
				});
			}
		}

		void setThreshold(double g)
		{
			_thr = g;
		}

	private:
		double _thr = 0.5;
};
