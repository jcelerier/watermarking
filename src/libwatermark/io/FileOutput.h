#pragma once
#include <sndfile.hh>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class FileOutput : public OutputManagerBase<data_type>
{
	public:
		FileOutput(const Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(cfg)
		{
		}

		void writeFile(const char * str)
		{
			auto v = MathUtil::interleave(this->data());

			auto format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
			auto channels = this->data().size();
			SndfileHandle outfile(str, SFM_WRITE, format, channels, (int) this->conf.samplingRate);

			outfile.write(v.data(),
						  v.size());
		}
};
