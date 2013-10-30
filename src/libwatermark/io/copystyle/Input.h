#pragma once

#include "CopyStyle.h"

template <typename data_type>
class Input: public CopyStyle<data_type>
{
	public:
		Input(const Parameters<data_type>& cfg):
			CopyStyle<data_type>(cfg)
		{

		}
		virtual ~Input() = default;

};
