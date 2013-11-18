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
			// Recopier cette ligne
			auto& channels = static_cast<CData<data_type>*>(data.get())->_data;

			// Petit exemple qui va multiplier tout par _gain.
			for(auto& channel : channels)
			{
				for(int i = 0; i < channel.size(); ++i)
				{
					channel[i] += 5000;
				}
			}

		}

		virtual void onDataUpdate() override
		{

		}

	private:
		double _gain = 0.1;
};
