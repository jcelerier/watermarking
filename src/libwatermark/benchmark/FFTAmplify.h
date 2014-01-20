#pragma once

#include "BenchmarkBase.h"

#include "properties/Amplitude.h"
#include "properties/FFTProperty.h"
template <typename data_type>
class FFTAmplify : public BenchmarkBase<data_type>, public AmplitudeProperty, public FFTProperty
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		FFTAmplify(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("FFTAmplify", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			// Recopier cette ligne
			auto& spectrum = getSpectrum<data_type>(data);

			for(auto& channel : spectrum)
			{
				for (auto& cplx : channel) // Dans l'ordre
				{
					double magnitude = std::abs(cplx) * _amplitude;
					double phase = std::arg(cplx);

					cplx = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
				}
			}
		}

};
