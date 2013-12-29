#pragma once
#include <algorithm>

#include "OutputOLA.h"

template <typename data_type>
/**
 * @brief The OutputFilter class
 *
 * Gère la copie depuis le buffer d'entrée.
 * Copie adaptée pour un filtrage avec k coefficients.
 *
 * Explications :
 * http://www.songho.ca/dsp/convolution/convolution.html
 */
class OutputFilter: public OutputOLA<data_type>
{
	public:
		OutputFilter(const unsigned int fs, const Parameters<data_type>& cfg):
			OutputOLA<data_type>(cfg),
			filterSize(fs)
		{
		}

		virtual typename CopyStyle<data_type>::size_type frameIncrement() final override
		{
			return this->conf.bufferSize - filterSize + 1;
		}

	private:
		unsigned int filterSize = 0;
};
