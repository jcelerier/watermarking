#pragma once
#include <algorithm>

#include "WatermarkBase.h"

/**
 * @brief The LSBEncode class
 *
 * Implémente l'encodage sur bit de poids faible.
 */
template <typename data_type>
class LSBEncode : public WatermarkBase<data_type>
{

	public:
		LSBEncode(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

    /*	virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& channelsData = getAudio<data_type>(data);
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
        }/**/

        virtual void operator()(Audio_p& data, WatermarkData& watermark) override
        {
            auto& channelsData = getAudio<data_type>(data);

            for(int j = 0; j < channelsData.size(); j++)
            {
                auto& sampleData = channelsData[j];
                for(int i = 0; i < sampleData.size(); ++i)
                {
                    short bit16 = 1;
                    for(int nb = 0; nb < nbBits; ++nb)
                    {
                        if(!watermark.isComplete())
                        {
                            bit16 = bit16 << nb;
                            short nonbit16 = (short) ~bit16;
                            short testBit = bit16 & ( (short)watermark.nextBit() << nb);
                            sampleData[i] = (sampleData[i] & nonbit16) | (testBit & bit16);
                        }
                    }
                }
            }
        }

        int nbBits = 3;
};
