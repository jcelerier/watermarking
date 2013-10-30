#pragma once

#include "CopyStyle.h"

template <typename data_type>
class Output: public CopyStyle<data_type>
{
	public:
		Output(const Parameters<data_type>& cfg):
			CopyStyle<data_type>(cfg)
		{

		}
		virtual ~Output() = default;
};
