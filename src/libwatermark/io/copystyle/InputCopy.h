#pragma once

#include "CopyStyle.h"

template <typename data_type>
class InputCopy: public CopyStyle<data_type>
{
	public:
		InputCopy(const Parameters<data_type>& cfg):
			CopyStyle<data_type>(cfg)
		{
		}

		virtual ~InputCopy() = default;
};

template <typename T>
using InputCopy_p = std::shared_ptr<InputCopy<T>>;
