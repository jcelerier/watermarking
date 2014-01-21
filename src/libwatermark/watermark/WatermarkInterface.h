#pragma once

#include "../Data.h"
#include "../watermarkdata/WatermarkData.h"

class WatermarkInterface
{
	public:
		virtual ~WatermarkInterface() = default;

		/**
		 * @brief operator ()
		 * @param data Données audio (samples ou spectre)
		 * @param watermark Données à watermarker.
		 */
		virtual void operator()(Audio_p& data, WatermarkData& watermark) = 0;

		virtual void finish(WatermarkData& ) {}
};

using Watermark_p = std::shared_ptr<WatermarkInterface>;
