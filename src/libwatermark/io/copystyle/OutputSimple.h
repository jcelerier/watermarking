#pragma once
#include <algorithm>

#include "OutputCopy.h"

template <typename data_type>
/**
 * @brief The OutputSimple class
 *
 * Gère la copie depuis le buffer de sortie.
 * Copie simple : On copie tout le buffer et on progresse de bufferSize
 */
class OutputSimple : public OutputCopy<data_type>
{
	public:
		using OutputCopy<data_type>::OutputCopy;

		virtual typename CopyStyle<data_type>::size_type frameIncrement() override
		{
			return CopyStyle<data_type>::conf.bufferSize;
		}

		/**
		 * @brief copy
		 * @param in Petit buffer
		 * @param out Grand buffer
		 * @param pos
		 * @param big_vector_length
		 */
		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) override
		{
			std::copy(in,
					  (frameIncrement() <= big_vector_length - pos) ?
						in + frameIncrement() :
						in + big_vector_length - frameIncrement(),
					  out + pos);
		}
};
