#pragma once
#include <sndfile.hh>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class FileOutput : public OutputManagerBase<data_type>
{
		using IOManagerBase<data_type>::v;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;
	public:
		FileOutput(Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(cfg)
		{
		}

		FileOutput(OutputCopy<data_type> * oc, Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(oc, cfg)
		{
		}

		void writeFile(const char * str)
		{
			auto tmp = MathUtil::interleave(v());

			SndfileHandle outfile(str, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, (int) channels(), (int) this->conf.samplingRate);

			outfile.write(tmp.data(),
						  tmp.size());
		}
};
