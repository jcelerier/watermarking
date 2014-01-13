#pragma once
#ifdef __APPLE__
#include "/usr/local/Cellar/libsndfile/1.0.25/include/sndfile.hh"
#else
#include "sndfile.hh"
#endif
#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
/**
 * @brief The FileOutput class
 *
 * Ecrit dans un fichier à l'aide de libsndfile
 */
class FileOutput : public OutputManagerBase<data_type>
{
		using IOManagerBase<data_type>::channels;
	public:
		using OutputManagerBase<data_type>::OutputManagerBase;

		void writeFile(const char * str)
		{
			auto tmp = MathUtil::interleave(this->v());

			SndfileHandle outfile(str, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, (int) channels(), (int) this->conf.samplingRate);

			outfile.write(tmp.data(),
						  tmp.size());
		}
};
