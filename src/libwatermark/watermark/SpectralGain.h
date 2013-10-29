#pragma once
#include <iostream>

#include "spectralwatermarkbase.h"
class SpectralGain : public SpectralWatermarkBase
{
	public:
		SpectralGain(const Parameters& configuration):
			SpectralWatermarkBase(configuration)
		{
		}

		virtual ~SpectralGain() = default;

		virtual SpectralWatermarkBase* clone() override
		{
			return new SpectralGain(*this);
		}

		virtual void operator()(IData* const data) override
		{
			std::vector<complex_type>& input_spectrum = dynamic_cast<CData<complex_type>*>(data)->_data;

			for (auto i = 0U; i < input_spectrum.size(); ++i)
			{
				double phase, power, magnitude;

				power = std::norm(input_spectrum[i]);
				phase = std::arg(input_spectrum[i]);

				power *= _gain;

				magnitude = std::sqrt(power);

				input_spectrum[i] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
			}

		}

		virtual void onFFTSizeUpdate() override
		{

		}

		virtual void onDataUpdate() override
		{

		}

	private :
		double _gain = 0.1;
};
