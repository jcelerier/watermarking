#pragma once
#include <vector>

#include "SpectralWatermarkBase.h"

/**
 * @brief The SSWDecode class
 *
 * Décodage SSW.
 */
template <typename data_type>
class SSWDecode : public SpectralWatermarkBase<data_type>
{
	public:
		SSWDecode(const Parameters<data_type>& configuration,
				  std::vector<int> & PNSequence,
				  std::vector<unsigned int> & freqWinIndexes,
				  double watermarkAmp):
			SpectralWatermarkBase<data_type>(configuration),
			_PNSequence(PNSequence),
			_freqWinIndexes(freqWinIndexes),
			_watermarkAmp(watermarkAmp)
		{
		}

		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& spectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;

			// produit scalaire normalisé entre (PNSequence * watermarkAmp) et spectrum[FreqIndexes]
			// Seuil pour décider si watermark présent ou pas

		}

	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp;
};
