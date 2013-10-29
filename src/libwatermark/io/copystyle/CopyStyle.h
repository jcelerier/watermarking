#pragma once

#include "../../Parameters.h"
#include <vector>

class CopyStyle
{
	public:
		using data_type = Parameters::data_type;
		using size_type = Parameters::size_type;

		CopyStyle(const Parameters& cfg):
			conf(cfg)
		{

		}

		virtual ~CopyStyle() = default;
		virtual void copy(std::vector<data_type>::const_iterator in,
				  std::vector<data_type>::iterator out,
				  size_type pos,
				  size_type in_length,
				  size_type out_length) = 0;


		virtual size_type frameIncrement() = 0;

	protected:
		const Parameters& conf;
};
