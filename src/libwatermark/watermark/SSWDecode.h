#pragma once
#include <vector>
#include <iostream>
#include "../benchmark/properties/FFTProperty.h"

#include "WatermarkBase.h"
#include "../mathutils/math_util.h"

/**
 * @brief The SSWDecode class
 *
 * Décodage SSW.
 */
template <typename data_type>
class SSWDecode : public WatermarkBase<data_type>, public FFTProperty
{
	public:
		SSWDecode(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{

		}

		SSWDecode(const Parameters<data_type>& configuration,
				  std::vector<int> & pPNSequence,
				  std::vector<unsigned int> & pfreqWinIndexes,
				  double pwatermarkAmp,
				  double pthreshold):
			WatermarkBase<data_type>(configuration),
			_PNSequence(pPNSequence),
			_freqWinIndexes(pfreqWinIndexes),
			_watermarkAmp(pwatermarkAmp),
			_threshold(pthreshold)
		{
		}

		virtual void operator()(Audio_p& data, WatermarkData& )  override
		{
			auto& channelsSpectrum = getSpectrum<data_type>(data);

			_channels_nb = channelsSpectrum.size();

			_correlations.push_back(std::vector<double>(_channels_nb));

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
					double power = 20.0 * std::log10(std::sqrt(std::norm(spectrumData[_freqWinIndexes[i]])) / (double) std::sqrt(this->conf.bufferSize));
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
				
				
				_correlations[_cpt][j] = correlation;
				
				//std::cout << "Corr : " << correlation << std::endl;
				if (_cpt != 0) {
					_mean *= (double) _cpt;
				}
				_mean += correlation;
				_mean /= (double) (_cpt + 1);

				if (correlation > _corr_max)
					_corr_max = correlation;
				if (correlation < _corr_min)
					_corr_min = correlation;

			}

			_cpt++;
			
		}

		virtual void finish(WatermarkData& watermark) override {
			
			char filename[64];

			FILE *file;
			
			double max_threshold = (_corr_max - _mean) * _threshold + _mean;
			double min_threshold = (_corr_min - _mean) * _threshold + _mean;
			

			for(unsigned int i = 0; i < _correlations.size(); i++) {
				
				std::vector<int> result;

				for(int j = 0; j < _channels_nb; j++) {
					
					sprintf(filename, "corr_channel%d", j);
					
					if (i == 0) {
						file = fopen(filename, "w");
						fprintf(file, "#Win Corr\n");
						fclose(file);
					}

					file = fopen(filename, "a");
					fprintf(file, "%d %f\n", _cpt, _correlations[i][j]);
					fclose(file);
					
					// Mémorisation de l'estimation du bit pour le canal j
					if (_correlations[i][j] > max_threshold) {
						// Corrélation proche de 1
						result.push_back(1);
					} else if (_correlations[i][j] < min_threshold) {
						// Corrélation proche de -1
						result.push_back(-1);
					} else {
						result.push_back(0);
					}
				}

				int detectedBit = result[0];

				// Comparaison des résultats sur tous les canaux
				for (int k = 1; k < result.size(); k++) {
					if (result[k] != detectedBit) {
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
		}


		std::vector<int> PNSequence() const
		{
			return _PNSequence;
		}

		void setPNSequence(const std::vector<int>& pPNSequence)
		{
			_PNSequence = pPNSequence;
		}
		std::vector<unsigned int> freqWinIndexes() const
		{
			return _freqWinIndexes;
		}

		void setFreqWinIndexes(const std::vector<unsigned int>& pfreqWinIndexes)
		{
			_freqWinIndexes = pfreqWinIndexes;
		}
		double watermarkAmp() const
		{
			return _watermarkAmp;
		}

		void setWatermarkAmp(double pwatermarkAmp)
		{
			_watermarkAmp = pwatermarkAmp;
		}

		double threshold() const
		{
		return _threshold;
		}

		void setThreshold(double pthreshold)
		{
		_threshold = pthreshold;
		}

	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp = 0;
		double _threshold = 0;
		int _cpt = 0;
		double _mean = 0.0;
		double _corr_max = 0.0;
		double _corr_min = 0.0;
		unsigned int _channels_nb = 0;
		std::vector< std::vector<double> > _correlations{};
};


