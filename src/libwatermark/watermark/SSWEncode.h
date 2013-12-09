#pragma once
#include <vector>

#include "SpectralWatermarkBase.h"

template <typename data_type>
/**
 * @brief The SSWEncode class
 *
 * Encodage SSW.
 */
class SSWEncode : public SpectralWatermarkBase<data_type>
{
	public:
		SSWEncode(const Parameters<data_type>& configuration,
				  std::vector<int> & PNSequence,
				  std::vector<unsigned int> & freqWinIndexes,
				  double watermarkAmp):
			SpectralWatermarkBase<data_type>(configuration),
			_PNSequence(PNSequence),
			_freqWinIndexes(freqWinIndexes),
			_watermarkAmp(watermarkAmp)
		{
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce sera un spectre.
		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			// Recopier cette ligne
			auto& channelsSpectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;

			if(!watermark.isComplete()) {

				double bit = (watermark.nextBit()) ? (1.0): (-1.0);

				for(int j = 0; j < channelsSpectrum.size(); j++)
				{
					auto& spectrumData = channelsSpectrum[j];

					for (int i = 0; i < _PNSequence.size(); i++)
					{
						double phase, power, magnitude;

						power = std::norm(spectrumData[_freqWinIndexes[i]]);
						phase = std::arg(spectrumData[_freqWinIndexes[i]]);

						magnitude = std::sqrt(power) + bit * _watermarkAmp * (double) _PNSequence[i];

						// Changer pour pouvoir utiliser plusieurs méthodes d'insertion
						spectrumData[_freqWinIndexes[i]] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
					}
				}
			}
		}

	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp;

};
