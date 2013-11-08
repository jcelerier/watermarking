#pragma once

#include "WindowBase.h"

template<typename data_type>
class BlackmanWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		BlackmanWindow(const Parameters<data_type>& cfg):
			WindowBase<data_type>(cfg)
		{
		}

		virtual void apply(std::vector<data_type>& v, const size_type increment) final override
		{
			const double f = 2.0 * M_PI / increment;
			const double g = 4.0 * M_PI / increment;
			for(auto i = 0U; i < increment; ++i)
			{
				v[i] *= 0.42 - 0.5 * cos(i * f) + 0.08 * cos(i * g);
			}
		}
};
