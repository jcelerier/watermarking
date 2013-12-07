#pragma once

#include "WatermarkBase.h"

/**
 * @brief The SpectralWatermarkBase class
 *
 *  Base class from which spectral watermark algorithms must inherit.
 */
template <typename data_type>
class SpectralWatermarkBase: public WatermarkBase<data_type>
{
	public:
		using complex_type = typename Parameters<data_type>::complex_type;

		SpectralWatermarkBase(const Parameters<data_type>& configuration):
			WatermarkBase<data_type>(configuration)
		{
		}

		virtual ~SpectralWatermarkBase() = default;
};
