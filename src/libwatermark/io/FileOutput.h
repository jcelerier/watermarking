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
			const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
			const int channels = 1;
			SndfileHandle outfile(str, SFM_WRITE, format, channels, OutputManagerBase<data_type>::conf.samplingRate);

			outfile.write(OutputManagerBase<data_type>::_baseData.data(),
						  OutputManagerBase<data_type>::_baseData.size());
		}
};
