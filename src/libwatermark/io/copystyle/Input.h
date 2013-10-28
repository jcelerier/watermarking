#pragma once
#include "CopyStyle.h"
#include <vector>

class Input: public CopyStyle
{
	public:
		Input(const Parameters& cfg):
			CopyStyle(cfg)
		{

		}
		virtual ~Input() = default;

};
