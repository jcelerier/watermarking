#pragma once
#include <algorithm>

#include "WatermarkBase.h"


// Ajoute du "buzz" ou un signal de basse fréquence au signal audio pour simuler l'impact d'une source d'alimentation.

template <typename data_type>
class AddBrumm : public WatermarkBase<data_type>
{

	public:
        AddBrumm(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce seront des samples, au format choisi (double, short...).
        virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			for(int j = 0; j < channelsData.size(); j++)
			{
				auto& sampleData = channelsData[j];
				for(int i = 0; i < sampleData.size(); ++i)
				{
                  sampleData[i] = sampleData[i] + sin(2*M_PI*i*50/conf.samplingRate);
				}
			}
		}

		virtual void onDataUpdate() override
		{

		}
};
