#pragma once
#include <vector>

#include "SpectralWatermarkBase.h"
#include "mathutils/math_util.h"

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
			auto& channelsSpectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;

			std::vector<int> result;

			for(int j = 0; j < channelsSpectrum.size(); j++)
			{
				auto& spectrumData = channelsSpectrum[j];

				// Séquence PN multipliée par l'amplitude du watermark
				std::vector<double> amplifiedPN;
				for (int i = 0; i < _PNSequence.size(); i++) {
					amplifiedPN.push_back(_watermarkAmp * _PNSequence[i]);
				}

				// Coefficients du spectre à modifier (sous forme de vecteur)
				std::vector<double> spectrumCoefs;
				for (int i = 0; i < _PNSequence.size(); i++) {
					spectrumCoefs.push_back(spectrumData[_freqWinIndexes[i]].real());
				}

				double PNnorm = MathUtil::norm_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), amplifiedPN.size());
				double coefsNorm = MathUtil::norm_n<std::vector<double>::iterator, double>(spectrumCoefs.begin(), spectrumCoefs.size());
				double correlation = (MathUtil::dotProduct_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), spectrumCoefs.begin(), amplifiedPN.size()))/(PNnorm * coefsNorm);

				if (correlation > _threshold) {
					// Corrélation proche de 1
					result.push_back(1);
				} else if (correlation < -_threshold) {
					// Corrélation proche de -1
					result.push_back(-1);
				} else {
					result.push_back(0);
				}
			}

			int detectedBit = result[0];

			// Comparaison des résultats sur tous les channels
			for (int i = 1; i < result.size(); i++) {
				if (result[i] != detectedBit) {
					return;
				}
			}

			if (detectedBit == 1)
				watermark.setNextBit(true);
			else if (detectedBit == -1)
				watermark.setNextBit(false);
		}

	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp;
		double _threshold;
};
