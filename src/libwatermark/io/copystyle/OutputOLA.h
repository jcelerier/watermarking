#pragma once
#include "Output.h"

class OutputOLA : public Output
{
	public:
		OutputOLA(const Parameters& cfg):
			Output(cfg)
		{
			_frame_increment = cfg.bufferSize / 2;
		}
		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
		{
			for (auto j = 0U; (j < in_length) && (pos + j < out_length); ++j)
			{
				out[pos + j] += in[j] / in_length; // Normalisation POUR FFTW ONLY !!!!
			}
		}
};
