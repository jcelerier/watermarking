#pragma once

#include "BenchmarkBase.h"
#include <algorithm>
#include <functional>
#include "../mathutils/math_util.h"

template <typename data_type>
class ComputeRMS : public BenchmarkBase<data_type>
{
	public:
		ComputeRMS(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("ComputeRMS", configuration)
		{
		}

		virtual void operator()(Audio_p& in) override
		{
			// 1. Faire soustraction entre les deux
			// 2. Calculer RMS De la soustraction

			// Pas de ref car on le modifie
			auto  in1 = getAudio<data_type>(getMulti(in)[0]);
			auto& in2 = getAudio<data_type>(getMulti(in)[1]);

			for(int i = 0; i < in1.size(); i++)
			{
				std::transform(in1[i].begin(),
							   in1[i].end(),
							   in2[i].begin(),
							   in1[i].begin(),
							   std::minus<data_type>());
				rms += MathUtil::RMS(in1[i].data(), in1[i].size());
			}
		}

		double rms = 0;
};
