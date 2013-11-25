#ifndef SSWENCODE_H
#define SSWENCODE_H

#include <cstdlib>
#include <vector>

#include "SpectralWatermarkBase.h"

// Algorithme d'encodage en SSW
template <typename data_type>
class SSWEncode : public SpectralWatermarkBase<data_type>
{
    public:
        SSWEncode(const Parameters<data_type>& configuration):
            SpectralWatermarkBase<data_type>(configuration)
        {
        }

        // La seule méthode importante est celle-ci.
        // data : les données audio. Ici ce sera un spectre.
        virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
        {

            // Recopier cette ligne
            auto& spectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;

            /*
            // Petit exemple qui va multiplier l'amplitude de chaque bande de spectre par _gain.
            for(auto& channel : spectrum)
            {
                for (auto i = 0U; i < channel.size(); ++i)
                {
                    double phase, power, magnitude;

                    power = std::norm(channel[i]);
                    phase = std::arg(channel[i]);

                    magnitude = std::sqrt(power) * _gain;

                    channel[i] = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
                }
            }
            */

        }

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
            vector<int> frequencyRange;

            int j = (rand()%(sampleRate-size))+1;
            frequencyRange.push_back(j);

            for(int i = 1; i < size; i++)
            {
                frequencyRange.push_back(i+j);
            }

            return frequencyRange;
        }

        virtual void onFFTSizeUpdate() override
        {

        }

        virtual void onDataUpdate() override
        {

        }

    private :
};

#endif // SSWENCODE_H
