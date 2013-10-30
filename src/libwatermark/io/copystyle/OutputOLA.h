#pragma once

#include "Output.h"

template <typename data_type>
class OutputOLA : public Output
{
	public:
		OutputOLA(const Parameters& cfg):
			Output(cfg)
		{

		}

		virtual size_type frameIncrement() final override
		{
			return conf.bufferSize / 2;
		}

		virtual void copy(typename std::vector<data_type>::const_iterator in,
						  typename std::vector<data_type>::iterator out,
						  typename CopyStyle<data_type>::size_type pos,
						  typename CopyStyle<data_type>::size_type in_length,
						  typename CopyStyle<data_type>::size_type out_length) final override
		{
			for (auto j = 0U; (j < in_length) && (pos + j < out_length); ++j)
			{
				out[pos + j] += in[j] / in_length; // Normalisation POUR FFTW ONLY !!!!
			}
		}
};
