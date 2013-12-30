#pragma once

#include "WindowBase.h"

template<typename data_type>
class BlackmanWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		BlackmanWindow():
			WindowBase<data_type>({2})
		{
		}

		virtual void generate(const size_type windowSize)
		{
			this->windowCoeffs.resize(windowSize);

			for(auto i = 0U; i < windowSize; ++i)
			{
				const double f = 2.0 * M_PI / windowSize;
				const double g = 4.0 * M_PI / windowSize;
				const double a0 = 7938.0 / 18608.0,
							 a1 = 9240.0 / 18608.0,
							 a2 = 1430.0 / 18608.0;

				this->windowCoeffs[i] = a0 - a1 * std::cos((double)i * f) + a2 * std::cos((double)i * g);
			}
		}
};
