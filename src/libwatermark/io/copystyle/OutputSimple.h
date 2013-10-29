#pragma once
#include "Output.h"
#include <algorithm>

class OutputSimple : public Output
{
	public:
		OutputSimple(const Parameters& cfg):
			Output(cfg)
		{

		}

		virtual size_type frameIncrement() final override
		{
			return conf.bufferSize;
		}

		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
		{
			if (in_length <= out_length - pos)
			{
				std::copy(in, in+in_length, out + pos);
			}
			else //fileSize - pos < fftSize
			{
				std::copy(in, in + out_length - in_length, out + pos);
			}
		}
};
