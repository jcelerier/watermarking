#pragma once
#include <algorithm>

#include "InputCopy.h"

template <typename data_type>
class InputOLA : public InputCopy<data_type>
{
	public:
		InputOLA(const Parameters<data_type>& cfg):
			InputCopy<data_type>(cfg)
		{
		}

		virtual typename InputCopy<data_type>::size_type frameIncrement() final override
		{
			return InputCopy<data_type>::conf.bufferSize / 2;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) final override
		{
			if (frameIncrement() <= big_vector_length - pos)
			{
				std::copy_n(in + pos, 2 * frameIncrement(), out);
				//std::fill_n(out + frameIncrement() * 2, frameIncrement() * 2, 0);
			}
			else
			{
				std::copy_n(in + pos, big_vector_length - pos, out);
				std::fill_n(out + big_vector_length - pos, frameIncrement() - (big_vector_length - pos), 0);
			}
		}

};
