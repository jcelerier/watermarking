#pragma once
#include "spectralwatermarkbase.h"

/**
 * @brief The SimpleSpectralSubtraction class
 *
 * Performs basic spectral subtraction.
 */
class BypassWatermark : public SpectralWatermarkBase
{
	public:
		BypassWatermark(const Parameters& configuration);
		~BypassWatermark() = default;
		virtual SpectralWatermarkBase* clone() override;

		/**
		 * @brief Performs spectral subtraction, simple algorithm.
		 *
		 * @param input_spectrum Input spectrum.
		 * @param noise_power Estimated noise power.
		 */
		virtual void operator()(IData* const input_spectrum) override;
		virtual void onFFTSizeUpdate() override;
		virtual void onDataUpdate() override;
};
