#pragma once
#include "output.h"

class OutputOLA
{
	public:
		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
		{
			for (auto j = 0U; (j < _fft->size()) && (pos + j < _tabLength); ++j)
			{
				_data[pos + j] += _fft->output()[j] / _fft->size();
			}
		}
};
