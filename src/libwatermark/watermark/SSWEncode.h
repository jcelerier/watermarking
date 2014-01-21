#pragma once
#include <vector>
#include <cstdio>

#include "../benchmark/properties/FFTProperty.h"
#include "WatermarkBase.h"

template <typename data_type>
/**
 * @brief The SSWEncode class
 *
 * Encodage SSW.
 */
class SSWEncode : public WatermarkBase<data_type>, public FFTProperty
{
	public:
		SSWEncode(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{

		}

		SSWEncode(const Parameters<data_type>& configuration,
				  std::vector<int> & pPNSequence,
				  std::vector<unsigned int> & pfreqWinIndexes,
				  double pwatermarkAmp):
			WatermarkBase<data_type>(configuration),
			_PNSequence(pPNSequence),
			_freqWinIndexes(pfreqWinIndexes),
			_watermarkAmp(pwatermarkAmp)
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

						power = std::sqrt(std::norm(spectrumData[_freqWinIndexes[i]]));
						phase = std::arg(spectrumData[_freqWinIndexes[i]]);

						power /= (double) std::sqrt(this->conf.bufferSize);

						// Changer pour pouvoir utiliser plusieurs méthodes d'insertion
						magnitudeDB = 20.0 * std::log10(power) + bit * _watermarkAmp * (double) _PNSequence[i];

						magnitude = std::pow(10.0, (magnitudeDB / 20.0));

						magnitude *= (double) std::sqrt(this->conf.bufferSize);

						spectrumData[_freqWinIndexes[i]] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
					}
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


	private :
		std::vector<int> _PNSequence = {};
		std::vector<unsigned int> _freqWinIndexes = {};
		double _watermarkAmp = 0;

};


