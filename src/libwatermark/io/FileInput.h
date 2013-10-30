#pragma once
#include <istream>
#include <fstream>

#include <sndfile.hh>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"


class FileInput : public InputManagerBase
{
	public:
		FileInput(std::string filename, const Parameters& cfg):
			InputManagerBase(cfg)
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

			_baseData.resize(myf.frames());
			myf.read(_baseData.data(), myf.frames());
		}
};
