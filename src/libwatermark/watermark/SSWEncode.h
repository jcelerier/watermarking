#pragma once
#include <vector>
#include <stdio.h>

#include "WatermarkBase.h"

template <typename data_type>
/**
 * @brief The SSWEncode class
 *
 * Encodage SSW.
 */
class SSWEncode : public WatermarkBase<data_type>
{
	public:
		SSWEncode(const Parameters<data_type>& configuration,
				  std::vector<int> & PNSequence,
				  std::vector<unsigned int> & freqWinIndexes,
				  double watermarkAmp):
			WatermarkBase<data_type>(configuration),
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
			auto& channelsSpectrum = getSpectrum<data_type>(data);
			if(!watermark.isComplete()) {

				double bit = (watermark.nextBit()) ? (1.0): (-1.0);

				for(int j = 0; j < channelsSpectrum.size(); j++)
				{
					auto& spectrumData = channelsSpectrum[j];

					for (int i = 0; i < _PNSequence.size(); i++)
					{
						double phase, power, magnitudeDB, magnitude;

						power = std::norm(spectrumData[_freqWinIndexes[i]]);
						phase = std::arg(spectrumData[_freqWinIndexes[i]]);

						// Changer pour pouvoir utiliser plusieurs méthodes d'insertion
						magnitudeDB = 20.0 * std::log10(std::sqrt(power)) + bit * _watermarkAmp * (double) _PNSequence[i];

						magnitude = std::pow(10.0, (magnitudeDB / 20.0));

						spectrumData[_freqWinIndexes[i]] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
					}

					/*

					std::vector<double> amplifiedPN;
					for (int i = 0; i < _PNSequence.size(); i++) {
						amplifiedPN.push_back(_watermarkAmp * (double) _PNSequence[i]);
					}

					// Coefficients du spectre à modifier (sous forme de vecteur des normes des complexes)
					std::vector<double> coefs_power;
					for (int i = 0; i < _PNSequence.size(); i++) {
						double power = 20.0 * std::log10(std::sqrt(std::norm(spectrumData[_freqWinIndexes[i]])));
						coefs_power.push_back(power);
					}

					// Calcul de la corrélation

					double PNnorm = MathUtil::norm_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), amplifiedPN.size());
					double coefsNorm = MathUtil::norm_n<std::vector<double>::iterator, double>(coefs_power.begin(), coefs_power.size());

					std::cout << "Coefs Norm : " << coefsNorm << " ";

					// A changer pour pouvoir utiliser d'autres correlations en fonction de la méthode d'insertion
					double correlation = (MathUtil::dotProduct_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), coefs_power.begin(), amplifiedPN.size()))/(PNnorm * coefsNorm);

					std::cout << "Corr : " << correlation << std::endl;

					*/
				}
			}
		}

	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp;

};
