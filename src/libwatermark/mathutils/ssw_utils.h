#pragma once
#include <cstdlib>
#include <vector>

namespace SSWUtil
{
	/* Génère une séquence pseudo-aléatoire à valeurs comprises dans {-1,1}
	   de taille size */
        std::vector<int> generatePNSequence(int size)
        {
            srand(time(0));
            vector<int> pnsequence;
	    
            for(int i = 0; i < size; i++)
            {
                int j = rand()%2;
                if(j == 0)
                    pnsequence.push_back(-1);
                else
                    pnsequence.push_back(1);
            }

            return pnsequence;
        }

        /* Génère une plage de fréquence aléatoire de taille size et comprise dans
            l'intervalle [1,sampleRate] */
        std::vector<int> generateFrequencyRange(int size, int sampleRate)
        {
            srand(time(0));
	    std::vector<int> frequencyRange;

            int j = (rand()%(sampleRate-size))+1;
            frequencyRange.push_back(j);

            for(int i = 1; i < size; i++)
            {
		    frequencyRange.push_back(i+j);
            }

            return frequencyRange;
        }

}
