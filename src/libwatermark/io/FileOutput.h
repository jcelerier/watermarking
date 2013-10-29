#pragma once
#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"
#include <ostream>
#include <fstream>
#include <algorithm>
#include "sndfile.hh"

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

			//			std::ofstream ofile(str, std::ios_base::ate | std::ios_base::binary);
			//			std::cerr << "final size : " << _baseData.size() << std::endl;
			//			ofile.write((char*)&buff, _baseData.size());

			//			short* buff = new short[_baseData.size()];
			//			std::transform(_baseData.begin(), _baseData.end(), buff, [] (data_type sample) { return MathUtil::DoubleToShort(sample); });

			outfile.write(_baseData.data(), _baseData.size());

			//			delete buff;

			//			ofile.close();
		}
};
