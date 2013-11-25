#pragma once
#include <algorithm>

#include "WatermarkBase.h"

// Exemple de comment faire un algo temporel.
template <typename data_type>
class LSBDecode : public WatermarkBase<data_type>
{

	public:
        LSBDecode(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce seront des samples, au format choisi (double, short...).
		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			// Recopier cette ligne
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;

			// Petit exemple qui va multiplier tout par _gain.
			for(int j = 0; j < channelsData.size(); j++)
			{
				auto& sampleData = channelsData[j];
				for(int i = 0; i < sampleData.size(); ++i)
				{
					short testBit = 0x0001 & sampleData[i];
					bool bit = (testBit != 0);
					watermark.bits.push_back(bit);					
				}
			}

		}

		virtual void onDataUpdate() override
		{

        }
};
