#pragma once
#include <sndfile.hh>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class FileOutput : public OutputManagerBase<data_type>
{
		using OutputManagerBase<data_type>::v;
	public:
		FileOutput(const Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(cfg)
		{
		}

		void writeFile(const char * str)
		{
			auto tmp = MathUtil::interleave(v());

			//std::cerr << v()[1][3] << " = " << tmp[7] << std::endl;
			auto format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
			int channels = (int) v().size();
			SndfileHandle outfile(str, SFM_WRITE, format, channels, (int) this->conf.samplingRate);

			outfile.write(tmp.data(),
						  tmp.size());
		}
};
