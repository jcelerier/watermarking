#pragma once
#include <algorithm>

#include "LSB/LSBBase.h"


/**
 * @brief The LSBEncode class
 *
 * Implémente l'encodage sur bit de poids faible.
 */
template <typename data_type>
class LSBEncode : public LSBBase<data_type>
{
	public:
		using LSBBase<data_type>::LSBBase;

        virtual void operator()(Audio_p& data, WatermarkData& watermark) override
        {
            auto& channelsData = getAudio<data_type>(data);

            for(int j = 0; j < channelsData.size(); j++)
            {
                auto& sampleData = channelsData[j];
                for(int i = 0; i < sampleData.size(); ++i)
                {
					for(int nb = 0; nb < this->nbBits; ++nb)
                    {
                        short bit16 = 1;
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
};
