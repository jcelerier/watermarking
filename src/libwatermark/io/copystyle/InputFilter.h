#pragma once
#include <algorithm>

#include "InputSimple.h"

template <typename data_type>
/**
 * @brief The InputFilter class
 *
 * Gère la copie depuis le buffer d'entrée.
 * Copie adaptée pour un filtrage par convolution avec un noyau à k coefficients.
 *
 * Explications :
 * http://www.songho.ca/dsp/convolution/convolution.html
 */
class InputFilter: public InputSimple<data_type>
{
	public:
		InputFilter(const unsigned int fs, const Parameters<data_type>& cfg):
			InputSimple<data_type>(cfg),
			filterSize(fs)
		{
		}

		virtual typename CopyStyle<data_type>::size_type frameIncrement() final override
		{
			return CopyStyle<data_type>::conf.bufferSize - filterSize + 1;
		}

	private:
		unsigned int filterSize = 0;
};
