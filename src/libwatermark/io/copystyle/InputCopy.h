#pragma once

#include "CopyStyle.h"

template <typename data_type>
class InputCopy: public CopyStyle<data_type>
{
	public:
		using CopyStyle<data_type>::CopyStyle;
		virtual ~InputCopy() = default;
};

template <typename T>
using InputCopy_p = std::shared_ptr<InputCopy<T>>;
