#pragma once

#include "CopyStyle.h"

class Input: public CopyStyle
{
	public:
		Input(const Parameters& cfg):
			CopyStyle(cfg)
		{

		}
		virtual ~Input() = default;

};
