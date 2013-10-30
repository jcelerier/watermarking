#pragma once
#include <complex>

#include "watermarkbase.h"

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
		virtual SpectralWatermarkBase<data_type>* clone() = 0;

		/**
		 * @brief Actions to perform if the FFT size changes.
		 *
		 * Most of the buffers will have to change.
		 */
		virtual void onFFTSizeUpdate() = 0;

		/**
		 * @brief Actions to perform if the data changes a lot.
		 *
		 * For instance, discard previous data saved by the algorithms which would not have sense anymore.
		 */
		virtual void onDataUpdate() = 0;

};
