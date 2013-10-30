#pragma once
#include <algorithm>

#include "Input.h"

template <typename data_type>
class InputOLA : public Input
{
	public:
		InputOLA(const Parameters& cfg):
			Input<data_type>(cfg)
		{
		}

		virtual size_type frameIncrement() final override
		{
			return conf.bufferSize / 2;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type in_length,
						  typename CopyStyle<data_type>::size_type out_length) final override
		{
			if (_frame_increment <= in_length - pos)
			{
				std::copy_n(in + pos, _frame_increment, out);
				std::fill_n(out + _frame_increment, _frame_increment, 0);
			}
			else
			{
				std::copy_n(in + pos, in_length - pos, out);
				std::fill_n(out + in_length - pos, _frame_increment - (in_length - pos), 0);

			}
		}

};
