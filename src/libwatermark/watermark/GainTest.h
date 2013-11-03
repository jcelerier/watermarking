#pragma once
#include <algorithm>

#include "watermarkbase.h"

// Exemple de comment faire un algo temporel.
template <typename data_type>
class GainTest : public WatermarkBase<data_type>
{
	public:
		GainTest(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		virtual GainTest* clone() override
		{
			return new GainTest<data_type>(*this);
		}

		// La seule méthode importante est celle-ci.
		// data : les données audio. Ici ce seront des samples, au format choisi (double, short...).
		virtual void operator()(IData* const data) override
		{
			// Recopier cette ligne
			auto& samples = dynamic_cast<CData<data_type>*>(data)->_data;

			// Petit exemple qui va multiplier tout le tableau par _gain.
			for(auto& channel : samples)
			{
				std::transform(channel.begin(),
						channel.end(),
						channel.begin(),
						[&] (data_type x) { return x * _gain; });
			}

		}

		virtual void onDataUpdate() override
		{

		}

	private:
		double _gain = 0.1;
};
