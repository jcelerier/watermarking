#pragma once
#include <algorithm>

#include "LSB/LSBBase.h"

/**
 * @brief The LSBDecode class
 *
 * Implémente le décodage sur bit de poids faible.
 */
template <typename data_type>
class LSBDecode : public LSBBase<data_type>
{
	public:
        LSBDecode(const Parameters<data_type>& configuration):
			LSBBase<data_type>(configuration)
		{
		}

       /* virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
        {
            auto& channelsData = getAudio<data_type>(data);

            for(int j = 0; j < channelsData.size(); j++)
            {
                auto& sampleData = channelsData[j];
                for(int i = 0; i < sampleData.size(); ++i)
                {
                        short testBit = 0x0001 & sampleData[i];
                        bool bit = (testBit != 0);
                        watermark.setNextBit(bit);
                }
            }
		}*/

		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
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
                        bit16 = bit16 << nb;
                        short testBit = bit16 & sampleData[i];
                        bool bit = (testBit != 0);
                        watermark.setNextBit(bit);
                    }
				}
			}
		}
};

