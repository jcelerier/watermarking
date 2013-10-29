#pragma once
#include "Input.h"
#include <algorithm>

class InputOLA : public Input
{
	public:
		InputOLA(const Parameters& cfg):
			Input(cfg)
		{

		}

		virtual size_type frameIncrement() final override
		{
			return conf.bufferSize / 2;
		}
		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
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
