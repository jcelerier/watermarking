#pragma once
#include <algorithm>
#include <random>

#include "BenchmarkBase.h"
#include "properties/FFTProperty.h"

template <typename data_type>
class FFTNoise : public BenchmarkBase<data_type>, public FFTProperty
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		FFTNoise(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("FFTNoise", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			// Recopier cette ligne
			auto& spectrum = getSpectrum<data_type>(data);

			for(auto& channel : spectrum)
			{
				for (auto i = 0U; i < channel.size(); ++i)
				{
					double phase, power, magnitude;

					power = std::norm(channel[i]);
					phase = std::arg(channel[i]);

					magnitude = std::sqrt(power) + noise_factor;

					channel[i] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
				}
			}
		}

	private:
		double noise_factor = 0.5;
};
