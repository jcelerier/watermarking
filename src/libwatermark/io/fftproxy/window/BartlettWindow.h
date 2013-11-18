#pragma once

#include "WindowBase.h"

// Fenêtre triangulaire
template<typename data_type>
class BartlettWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		BartlettWindow(const Parameters<data_type>& cfg):
			WindowBase<data_type>(cfg)
		{
		}

		virtual void apply(std::vector<data_type>& v, const size_type increment) final override
		{
			const double f = 2.0 * M_PI / increment;
			for(auto i = 0U; i < increment; ++i)
			{
				v[i] *= (i < increment / 2) ?
							2.0 * (double)i / increment :
							2.0 * (double)(increment - i) / increment;
			}
		}
};
