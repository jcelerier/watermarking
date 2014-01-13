#pragma once
#include "WatermarkBase.h"

/**
 * @brief The DummyWatermark class
 *
 * Implémente l'encodage sur bit de poids faible.
 */
template <typename data_type>
class DummyWatermark : public WatermarkBase<data_type>
{

	public:
		DummyWatermark(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		virtual void operator()(Audio_p& buf , WatermarkData& )  override
		{
			auto truc = getSpectrum<data_type>(buf);

			for(int i = 0; i < 6; i++)
			{
				std::cerr << truc[0][i] << std::endl;
			}
			std::cerr << std::endl;
			return;
		}

};
