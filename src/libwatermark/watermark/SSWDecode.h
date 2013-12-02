#ifndef SSWDECODE_H
#define SSWDECODE_H

#include <cstdlib>
#include <vector>

#include "SpectralWatermarkBase.h"

// Algorithme de décodate de la méthode SSW
template <typename data_type>
class SSWDecode : public SpectralWatermarkBase<data_type>
{
    public:
        SSWDecode(const Parameters<data_type>& configuration, 
		  std::vector<int> & PNSequence,
		  std::vector<int> & freqWinIndexes,
		  double watermarkAmp):
		SpectralWatermarkBase<data_type>(configuration),
		_PNSequence(PNSequence), _freqWinIndexes(freqWinIndexes),
		_watermarkAmp(watermarkAmp)
	{
        }

        // La seule méthode importante est celle-ci.
        // data : les données audio. Ici ce sera un spectre.
        virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
        {

            // Recopier cette ligne
            auto& spectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;
	    
	    // produit scalaire normalisé entre (PNSequence * watermarkAmp) et spectrum[FreqIndexes]
	    // Seuil pour décider si watermark présent ou pas

        }

        virtual void onFFTSizeUpdate() override
        {
		
        }

        virtual void onDataUpdate() override
        {
		
        }

    private :
	double _watermarkAmp;
	std::vector<int> _freqWinIndexes;
	std::vector<int> _PNSequence;

};

#endif // SSWDECODE_H
