#pragma once
#include <algorithm>

#include "WatermarkBase.h"
#include <vector>
#include <math.h>

/**
 * @brief The DLSBDecode class
 *
 * Implémente le décodage sur bit de poids faible.
 */
template <typename data_type>
class RLSBDecode : public WatermarkBase<data_type>
{
	public:
        RLSBDecode(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
		{
			auto& channelsData = getAudio<data_type>(data);

            std::vector <std::vector <bool>> tabBit (nbBits);

			for(int j = 0; j < channelsData.size(); j++)
			{
				auto& sampleData = channelsData[j];
                for(int i = 0; i < sampleData.size(); ++i)
				{
                    for(int nb = 0; nb < nbBits; ++nb)
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
               }/**/
			}


            /*

            int nbTot = 0;
            double moyenne = 0;

            for(int j = 0; j < channelsData.size(); ++j)
            {
                auto& sampleData = channelsData[j];
                for(int i = 0; i < sampleData.size(); ++i)
                {
                   for(int nb = 0; nb < nbBits; ++nb)
                    {
                        nbTot += nb;
                        moyenne += (double)tabBit[nb][i]*nb;
                    }
                   moyenne = moyenne / nbTot;
                   if ( moyenne < 0.5)
                   {
                       moyenne = floor(moyenne);
                   }
                   else
                   {
                       moyenne = ceil(moyenne);
                   }
                   bool bit = (bool)moyenne;
                   watermark.setNextBit(bit);
                   int nbTot = 0;
                   double moyenne = 0;
                }
            }*/
        }



        int nbBits = 6; // pour tester le décodage, il faut tester uniquement la ligne de bits désirée (nbBits = 1 et
                        // nb du for = 0, nbBits = 2 et nb du for = 1 ... )
};

