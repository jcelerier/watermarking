#pragma once

#include "WindowBase.h"

// Fenêtre triangulaire
template<typename data_type>
class BartlettWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		BartlettWindow():
			WindowBase<data_type>({2})
		{
		}

		virtual void generate(const size_type windowSize)
		{
			this->windowCoeffs.resize(windowSize);

			for(auto i = 0U; i < windowSize; ++i)
			{
				this->windowCoeffs[i] = (i < windowSize / 2) ?
							2.0 * (double)i / windowSize :
							2.0 * (double)(windowSize - i) / windowSize;
			}
		}
};
