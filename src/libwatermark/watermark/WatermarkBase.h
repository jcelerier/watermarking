#pragma once

#include "../Parameters.h"
#include "../Data.h"
#include "../watermarkdata/WatermarkData.h"



/**
 * @brief The WatermarkBase class
 *
 * Classe de base pour les algorithmes de watermark.
 * TODO : faire une méta-classe AudioFX qui ne précise pas de foncteur.
 */
template <typename data_type>
class WatermarkBase
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		WatermarkBase(const Parameters<data_type>& configuration):
			conf(configuration)
		{
		}

		/**
		 * @brief operator ()
		 * @param data Données audio (samples ou spectre)
		 * @param watermark Données à watermarker.
		 */
		virtual void operator()(Audio_p& data, WatermarkData& watermark) = 0;

		virtual ~WatermarkBase() = default;

	protected:
		const Parameters<data_type>& conf;
};

template<typename T>
using Watermark_p = std::shared_ptr<WatermarkBase<T>>;
