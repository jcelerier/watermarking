#pragma once
#include <iostream>

#include "spectralwatermarkbase.h"

template <typename data_type>
class SpectralGain : public SpectralWatermarkBase<data_type>
{
	public:
		SpectralGain(const Parameters<data_type>& configuration):
			SpectralWatermarkBase<data_type>(configuration)
		{
		}

		virtual ~SpectralGain() = default;

		virtual SpectralWatermarkBase<data_type>* clone() override
		{
			return new SpectralGain<data_type>(*this);
		}

		virtual void operator()(IData* const data) override
		{
			std::vector<typename SpectralWatermarkBase<data_type>::complex_type>& input_spectrum = dynamic_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data)->_data;

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
