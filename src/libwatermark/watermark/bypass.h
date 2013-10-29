#pragma once
#include "spectralwatermarkbase.h"
#include <iostream>
/**
 * @brief The SimpleSpectralSubtraction class
 *
 * Performs basic spectral subtraction.
 */
class BypassWatermark : public SpectralWatermarkBase
{
	public:
		BypassWatermark(const Parameters& configuration):
			SpectralWatermarkBase(configuration)
		{
		}

		virtual ~BypassWatermark() = default;

		virtual SpectralWatermarkBase* clone() override
		{
			return new BypassWatermark(*this);
		}

		virtual void operator()(IData* const data) override
		{
			/*
			std::vector<complex_type>& input_spectrum = dynamic_cast<CData<complex_type>*>(data)->_data;

			for (auto i = 0U; i < input_spectrum.size(); ++i)
			{
				double phase, power, magnitude;

				power = std::norm(input_spectrum[i]);
				phase = std::arg(input_spectrum[i]);

				if(i % 4) power += 10;

				magnitude = std::sqrt(power);

				input_spectrum[i] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
			}
			*/
		}

		virtual void onFFTSizeUpdate() override
		{

		}

		virtual void onDataUpdate() override
		{

		}
};
