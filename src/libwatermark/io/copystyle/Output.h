#pragma once

#include "CopyStyle.h"

class Output: public CopyStyle
{
	public:
		Output(const Parameters& cfg):
			CopyStyle(cfg)
		{

		}
		virtual ~Output() = default;
};
