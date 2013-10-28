#pragma once

#include "../IOManagerBase.h"
#include <vector>

class CopyStyle
{
	public:
		using data_type = double;
		using size_type = long unsigned int;
		virtual ~CopyStyle() = default;
		virtual void copy(std::vector<data_type>::const_iterator in,
				  std::vector<data_type>::iterator out,
				  size_type pos,
				  size_type in_length,
				  size_type out_length);


		size_type frameIncrement()
		{
			return _frame_increment;
		}

	protected:
		size_type _frame_increment = 0;

};
