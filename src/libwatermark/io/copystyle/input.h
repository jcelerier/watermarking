#pragma once
#include "../IOManagerBase.h"
#include <vector>
class Input
{
	protected:
		using data_type = double;
		using size_type = unsigned int;

	public:
		void copy(std::vector<data_type>::const_iterator in,
				  std::vector<data_type>::iterator out,
				  size_type pos,
				  size_type in_length,
				  size_type out_length);




		virtual ~Input() = default;



};
