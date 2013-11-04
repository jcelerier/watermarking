#pragma once

#include "Output.h"

template <typename data_type>
class OutputOLA : public Output<data_type>
{
	public:
		OutputOLA(const Parameters<data_type>& cfg):
			Output<data_type>(cfg)
		{

		}

		virtual typename Output<data_type>::size_type frameIncrement() final override
		{
			return Output<data_type>::conf.bufferSize / 2;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type big_vector_length) final override
		{
			for (auto j = 0U; (j < frameIncrement()) && (pos + j < big_vector_length); ++j)
			{
				out[pos + j] += in[j];
			}
		}
};
