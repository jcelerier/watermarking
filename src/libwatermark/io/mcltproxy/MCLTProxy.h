#pragma once


#include "../OutputManagerBase.h"
#include "../../mathutils/MCLT.h"

template <typename data_type>
class MCLTProxy
{
	protected:
		const Parameters<data_type>& conf;

	public:
		MCLTManager mclt;
		using complex_type = typename Parameters<data_type>::complex_type;

		MCLTProxy(const Parameters<data_type>& cfg):
			conf(cfg)
		{
		}

		virtual ~MCLTProxy() = default;
};
