#pragma once
#include <algorithm>

#include "LSB/LSBBase.h"
#include <vector>
#include <math.h>

/**
 * @brief The DLSBDecode class
 *
 * Implémente le décodage sur bit de poids faible.
 */
template <typename data_type>
class RLSBDecode : public LSBBase<data_type>
{
	public:
        RLSBDecode(const Parameters<data_type>& configuration):
			LSBBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& channelsData = getAudio<data_type>(data);

			std::vector <std::vector <bool>> tabBit (this->nbBits);

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

                        tabBit[nb].push_back(bit);
                        //watermark.setNextBit(bit);
                   }
				}
               for(int i = 0; i < sampleData.size(); ++i)
               {
                    watermark.setNextBit(tabBit[3][i]);
			   }
			}
		}
};

