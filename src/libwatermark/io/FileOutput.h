#pragma once
#include <sndfile.hh>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

class FileOutput : public OutputManagerBase
{
	public:
		FileOutput(const Parameters& cfg):
			OutputManagerBase(cfg)
		{

		}

		void writeFile(const char * str)
		{
			const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
			const int channels = 1;
			SndfileHandle outfile(str, SFM_WRITE, format, channels, conf.samplingRate);

			outfile.write(_baseData.data(), _baseData.size());
		}
};
