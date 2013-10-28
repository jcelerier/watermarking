#pragma once
#include "input.h"

class InputOLA
{
	public:
		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
		{
			// Data copying
			if (_ola_frame_increment <= _tabLength - pos) // last case
			{
				std::copy_n(_data + pos, _ola_frame_increment, _fft->input());
				std::fill_n(_fft->input() + _ola_frame_increment, _ola_frame_increment, 0);

				std::fill_n(_data + pos, _ola_frame_increment, 0);
			}
			else
			{
				std::copy_n(_data + pos, _tabLength - pos, _fft->input());
				std::fill_n(_fft->input() + _tabLength - pos, _ola_frame_increment - (_tabLength - pos), 0);

				std::fill_n(_data + pos, _tabLength - pos, 0);
			}
		}

};
