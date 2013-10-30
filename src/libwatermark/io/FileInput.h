#pragma once
#include <istream>
#include <fstream>

#include <sndfile.hh>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class FileInput : public InputManagerBase<data_type>
{
	public:
		FileInput(std::string filename, const Parameters<data_type>& cfg):
			InputManagerBase<data_type>(cfg)
		{
			readFile(filename.c_str());
		}

		/**
		 * @brief Reads a file into the internal buffer.
		 *
		 * @param str Path to the file.
		 * @return unsigned int Size of the file.
		 */
		void readFile(const char * str)
		{
			SndfileHandle myf = SndfileHandle(str);

			InputManagerBase<data_type>::_baseData.resize(myf.frames());
			myf.read(InputManagerBase<data_type>::_baseData.data(), myf.frames());
		}
};
