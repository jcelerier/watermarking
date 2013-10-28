#pragma once
#include "input.h"
#include <algorithm>

class InputSimple: public Input
{
	public:
		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
		{
			// Data copying
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
