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

		virtual void operator()(Audio_p& , WatermarkData& )  override
		{
		}

};
