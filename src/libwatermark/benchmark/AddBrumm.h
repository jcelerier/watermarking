#pragma once
#include <algorithm>

#include "BenchmarkBase.h"


// Ajoute du "buzz" ou un signal de basse fréquence au signal audio pour simuler l'impact d'une source d'alimentation.

template <typename data_type>
class AddBrumm : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
        AddBrumm(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			double const cst = 2.0 * M_PI * _frequence / conf.samplingRate;

			// Pour chaque canal (gauche - droite)
			for(auto& sampleData : channelsData)
			{
				// Pour chaque échantillon (dans l'ordre)
				for(auto& sample : sampleData)
				{
					sample = _amplitude * (sample + sin(cst * pos));
					pos = (pos == taille - 1) ? 0 : pos + 1;
				}
			}
		}

		void setFreq(int freq)
		{
			_frequence = freq;
			taille = conf.samplingRate / freq;
		}

		void setAmpli(double amp)
		{
			_amplitude = amp;
		}

	private:
		unsigned int _frequence = 50;
		data_type _amplitude = 1.0;
		size_type pos = 0;
		size_type taille = 44100 / 50;
};
