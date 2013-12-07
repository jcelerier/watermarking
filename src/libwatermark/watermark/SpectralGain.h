#pragma once
#include <iostream>

#include "SpectralWatermarkBase.h"

/**
 * @brief The SpectralGain class
 *
 * Example d'algorithme de réduction de volume par action sur
 * les bandes de magnitude. Sera supprimé quand SSW aura été implémenté.
 */
template <typename data_type>
class SpectralGain : public SpectralWatermarkBase<data_type>
{
	public:
		SpectralGain(const Parameters<data_type>& configuration):
			SpectralWatermarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data, WatermarkData& /*watermark*/)  override
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

	private :
		double _gain = 0.1;
};
