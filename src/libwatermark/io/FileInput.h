#pragma once
#include "InputManagerBase.h"
#include "../mathutils/math_util.h"
#include <istream>
#include <fstream>
#include <algorithm>

class FileInput : public InputManagerBase
{
	public:
		FileInput(std::string filename)
		{
			readFile(filename.c_str());
		}

		/**
		 * @brief Reads a file into the internal buffer.
		 *
		 * @param str Path to the file.
		 * @return unsigned int Size of the file.
		 */
		// Utiliser sndfile, fmod...
		void readFile(const char * str)
		{
			std::ifstream ifile(str, std::ios_base::ate | std::ios_base::binary);
			_baseData.resize(((unsigned int) ifile.tellg()) / (unsigned int) (sizeof(short) / sizeof(char)));
			ifile.clear();
			ifile.seekg(0, std::ios_base::beg);

			unsigned int pos = 0;
			short sample;

			while (ifile.read((char *)&sample, sizeof(short)))
			{
				_baseData[pos++] = MathUtil::ShortToDouble(sample);
			}

			ifile.close();
		}
};
