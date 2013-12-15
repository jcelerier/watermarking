#pragma once
#include <algorithm>

#include "OutputSimple.h"

template <typename data_type>
/**
 * @brief The OutputFilter class
 *
 * Gère la copie depuis le buffer d'entrée.
 * Copie adaptée pour un filtrage avec k coefficients
 */
class OutputFilter: public OutputCopy<data_type>
{
	public:
		OutputFilter(const unsigned int fs, const Parameters<data_type>& cfg):
			OutputCopy<data_type>(cfg),
			filterSize(fs)
		{
		}

		virtual typename CopyStyle<data_type>::size_type frameIncrement() final override
		{
			return this->conf.bufferSize - filterSize + 1;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) final override
		{
			for (auto j = 0U; (j < this->conf.bufferSize) && (pos + j < big_vector_length); ++j)
			{
				out[pos + j] += in[j];
			}
		}
	private:
		unsigned int filterSize = 0;
};
