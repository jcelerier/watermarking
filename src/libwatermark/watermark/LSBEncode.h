#pragma once
#include <algorithm>

#include "WatermarkBase.h"

// Exemple de comment faire un algo temporel.
template <typename data_type>
class LSBEncode : public WatermarkBase<data_type>
{

	public:
		LSBEncode(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce seront des samples, au format choisi (double, short...).
		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& channelsData = static_cast<CData<data_type>*>(data.get())->_data;
			short bit16 = 0x0001;
			short nonbit16 = (short) ~bit16;

			for(int j = 0; j < channelsData.size(); j++)
			{
				auto& sampleData = channelsData[j];
				for(int i = 0; i < sampleData.size(); ++i)
				{
					if(!watermark.isComplete())
					{
						short testBit = 0x0001 & (short)watermark.nextBit();
						sampleData[i] = (sampleData[i] & nonbit16) | (testBit & bit16);
					}
				}
			}
		}

		virtual void onDataUpdate() override
		{

		}
};
