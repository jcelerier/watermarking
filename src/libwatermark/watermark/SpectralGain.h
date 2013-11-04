#pragma once
#include <iostream>

#include "spectralwatermarkbase.h"

// Exemple de comment faire un algo spectral.
template <typename data_type>
class SpectralGain : public SpectralWatermarkBase<data_type>
{
	public:
		SpectralGain(const Parameters<data_type>& configuration):
			SpectralWatermarkBase<data_type>(configuration)
		{
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce sera un spectre.
		virtual void operator()(Audio_p data) override
		{
			// Recopier cette ligne
			auto& spectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;

			// Petit exemple qui va multiplier l'amplitude de chaque bande de spectre par _gain.
			for(auto& channel : spectrum)
			{
				for (auto i = 0U; i < channel.size(); ++i)
				{
					double phase, power, magnitude;

					power = std::norm(channel[i]);
					phase = std::arg(channel[i]);

					magnitude = std::sqrt(power) * _gain;

					channel[i] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
				}
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
