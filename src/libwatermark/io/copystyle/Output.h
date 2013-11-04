#pragma once

#include "CopyStyle.h"

template <typename data_type>
class OutputCopy: public CopyStyle<data_type>
{
	public:
		OutputCopy(const Parameters<data_type>& cfg):
			CopyStyle<data_type>(cfg)
		{
		}

		virtual ~OutputCopy() = default;
};

template <typename T>
using OutputCopy_p = std::shared_ptr<OutputCopy<T>>;
