#pragma once
#include <algorithm>

#include "Output.h"

template <typename data_type>
class OutputSimple : public OutputCopy<data_type>
{
	public:
		OutputSimple(const Parameters<data_type>& cfg):
			OutputCopy<data_type>(cfg)
		{
		}

		virtual typename CopyStyle<data_type>::size_type frameIncrement() final override
		{
			return CopyStyle<data_type>::conf.bufferSize;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) final override
		{
			std::copy(in,
					  (frameIncrement() <= big_vector_length - pos) ?
						in + frameIncrement() :
						in + big_vector_length - frameIncrement(),
					  out + pos);
		}
};
