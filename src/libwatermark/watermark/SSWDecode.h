#pragma once
#include <vector>
#include <iostream>

#include "WatermarkBase.h"
#include "mathutils/math_util.h"

/**
 * @brief The SSWDecode class
 *
 * Décodage SSW.
 */
template <typename data_type>
class SSWDecode : public WatermarkBase<data_type>
{
	public:
		SSWDecode(const Parameters<data_type>& configuration,
				  std::vector<int> & PNSequence,
				  std::vector<unsigned int> & freqWinIndexes,
				  double watermarkAmp, double threshold):
			WatermarkBase<data_type>(configuration),
			_PNSequence(PNSequence),
			_freqWinIndexes(freqWinIndexes),
			_watermarkAmp(watermarkAmp),
			_threshold(threshold)
		{
		}

		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& channelsSpectrum = getSpectrum<data_type>(data);

			std::vector<int> result;

			// Pour tous les canaux (2 pour du stéréo)
			for(int j = 0; j < channelsSpectrum.size(); j++)
			{
				// Récupération du spectre pour ce canal
				auto& spectrumData = channelsSpectrum[j];

				// Séquence PN multipliée par l'amplitude du watermark
				std::vector<double> amplifiedPN;
				for (int i = 0; i < _PNSequence.size(); i++) {
					amplifiedPN.push_back(_watermarkAmp * (double) _PNSequence[i]);
				}

				// Coefficients du spectre à modifier (sous forme de vecteur des normes des complexes)
				std::vector<double> coefs_power;
				for (int i = 0; i < _PNSequence.size(); i++) {
					//std::cout << spectrumData[_freqWinIndexes[i]] << " ";
					double power = 20.0 * std::log10(std::sqrt(std::norm(spectrumData[_freqWinIndexes[i]])));
					coefs_power.push_back(power);
				}
				//std::cout << std::endl;

				// Calcul de la corrélation

				double PNnorm = MathUtil::norm_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), amplifiedPN.size());
				double coefsNorm = MathUtil::norm_n<std::vector<double>::iterator, double>(coefs_power.begin(), coefs_power.size());

				//std::cout << "PN Norm : " << PNnorm << std::endl;
				//std::cout << "Coefs Norm : " << coefsNorm << " ";

				// A changer pour pouvoir utiliser d'autres correlations en fonction de la méthode d'insertion
				double correlation = (MathUtil::dotProduct_n<std::vector<double>::iterator, double>(amplifiedPN.begin(), coefs_power.begin(), amplifiedPN.size()))/(PNnorm * coefsNorm);

				//std::cout << "Corr : " << correlation << std::endl;

				char filename[64];
				sprintf(filename, "corr_channel%d", j);

				if (_cpt == 0) {
					FILE *file = fopen(filename, "w");
					
					fprintf(file, "#Win Corr\n");

					fclose(file);
				}

				FILE *file = fopen(filename, "a");
				
				fprintf(file, "%d %f\n", _cpt, correlation);
				
				fclose(file);

				// Mémorisation de l'estimation du bit pour le canal j
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

			_cpt++;

			int detectedBit = result[0];

			// Comparaison des résultats sur tous les canaux
			for (int i = 1; i < result.size(); i++) {
				if (result[i] != detectedBit) {
					return;
				}
			}

			// Si watermark détecté, insérer le bit dans le tableau en output
			if (detectedBit == 1) {
				watermark.setNextBit(true);
				//std::cout << "true" << "\n";
			} else if (detectedBit == -1) {
				watermark.setNextBit(false);
				//std::cout << "false" << "\n";
			}
		}

	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp;
		double _threshold;
		int _cpt = 0;
};
