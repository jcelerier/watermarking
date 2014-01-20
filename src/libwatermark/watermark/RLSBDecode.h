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
		using LSBBase<data_type>::LSBBase;

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
                   }
				}
               for(int i = 0; i < sampleData.size(); ++i)
               {
                   int sommeun = 0;
                   int sommezero = 0;
                   for (int nb = 0; nb < this->nbBits; ++nb)
                   {

                      if(tabBit[nb][i])
                          sommeun++;
                      else
                          sommezero++;

                      if(sommeun>sommezero)
                        watermark.setNextBit(true);
                      else
                        watermark.setNextBit(false);
                   }
			   }
			}
		}
};

