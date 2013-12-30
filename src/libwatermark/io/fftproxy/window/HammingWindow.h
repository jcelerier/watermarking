#pragma once

#include "WindowBase.h"

template<typename data_type>
class HammingWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		HammingWindow():
			WindowBase<data_type>({2, 1})
		{
		}

		virtual void generate(const size_type windowSize)
		{
			this->windowCoeffs.resize(windowSize);
			const double f = 2.0 * M_PI / (double)windowSize;

			for(auto i = 0U; i < windowSize; ++i)
			{
				this->windowCoeffs[i] = 0.54 - 0.46 * std::cos((double)i * f);
			}
		}
};
