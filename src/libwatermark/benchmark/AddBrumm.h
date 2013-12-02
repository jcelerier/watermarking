#pragma once
#include <algorithm>

#include "BenchmarkBase.h"


// Ajoute du "buzz" ou un signal de basse fréquence au signal audio pour simuler l'impact d'une source d'alimentation.

template <typename data_type>
class AddBrumm : public BenchmarkBase<data_type>
{

	public:
        AddBrumm(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>(configuration)
		{
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce seront des samples, au format choisi (double, short...).
		virtual void operator()(Audio_p& data)  override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			double const cst = 2.0 * M_PI * _frequence / this->conf.samplingRate;
			for(auto& sampleData : channelsData)
			{
				for(auto& sample : sampleData)
				{
					if(pos == taille) pos = 0;
					sample = _amplitude * (sample + sin(cst * pos));
					pos++;
				}
			}
		}

		void setFreq(int freq)
		{
			_frequence = freq;
			taille = this->conf.samplingRate / freq;
		}

		void amplitude(double amp)
		{
			_amplitude = amp;
		}

	private:
		int _frequence = 50;
		double _amplitude = 1.0;
		int pos = 0;
		int taille = 44100 / 800;
};
