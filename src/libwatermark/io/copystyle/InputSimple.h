#pragma once
#include <algorithm>

#include "Input.h"

template <typename data_type>
class InputSimple: public Input<data_type>
{
	public:
		InputSimple(const Parameters<data_type>& cfg):
			Input<data_type>(cfg)
		{
		}

		virtual typename CopyStyle<data_type>::size_type frameIncrement() final override
		{
			return CopyStyle<data_type>::conf.bufferSize;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type in_length,
						  typename CopyStyle<data_type>::size_type out_length) final override
		{
			if (out_length <= in_length - pos)
			{
				std::copy_n(in + pos, out_length, out);
			}
			else
			{
				std::copy_n(in + pos, in_length - pos, out);
				std::fill_n(out + in_length - pos, out_length - (in_length - pos), 0);
			}
		}

};
