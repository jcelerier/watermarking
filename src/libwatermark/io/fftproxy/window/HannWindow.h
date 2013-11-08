#pragma once

#include "WindowBase.h"

template<typename data_type>
class HannWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		HannWindow(const Parameters<data_type>& cfg):
			WindowBase<data_type>(cfg)
		{
		}

		virtual void apply(std::vector<data_type>& v, const size_type increment) final override
		{
			const double f = 2.0 * M_PI / (double)increment;
			for(auto i = 0U; i < increment; ++i)
			{
				v[i] *= 0.5 - 0.5 * std::cos((double)i * f);
			}
		}
};
