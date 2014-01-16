#pragma once
#include <algorithm>

#include "InputCopy.h"

template <typename data_type>
/**
 * @brief The InputSimple class
 *
 * Gère la copie depuis le buffer d'entrée.
 * Copie simple : on remplit le buffer d'entrée et on progresse de bufferSize
 */
class InputSimple: public InputCopy<data_type>
{
	public:
		using InputCopy<data_type>::InputCopy;

		virtual typename CopyStyle<data_type>::size_type frameIncrement() override
		{
			return CopyStyle<data_type>::conf.bufferSize;
		}

		/**
		 * @brief copy
		 * @param in Grand buffer
		 * @param out Petit buffer
		 * @param pos
		 * @param big_vector_length
		 */
		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) override
		{
			if (frameIncrement() <= big_vector_length - pos)
			{
				std::copy_n(in + pos, frameIncrement(), out);
			}
			else
			{
				std::copy_n(in + pos, big_vector_length - pos, out);
				std::fill_n(out + big_vector_length - pos, frameIncrement() - (big_vector_length - pos), 0);
			}
		}
};
