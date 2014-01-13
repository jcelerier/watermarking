#pragma once
#include <memory>

#include "../Parameters.h"
#include "WatermarkInterface.h"
/**
 * @brief The WatermarkBase class
 *
 * Classe de base pour les algorithmes de watermark.
 * TODO : faire une méta-classe AudioFX qui ne précise pas de foncteur.
 */
template <typename data_type>
class WatermarkBase: public WatermarkInterface
{
	public:
		WatermarkBase(const Parameters<data_type>& configuration):
			conf(configuration)
		{
		}

		virtual ~WatermarkBase() = default;

	protected:
		const Parameters<data_type>& conf;
};

