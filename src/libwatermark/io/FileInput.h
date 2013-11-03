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

		void readFile(const char * str)
		{
			auto myf = SndfileHandle(str);

			std::vector<data_type> vec;
			vec.resize(myf.frames() * myf.channels());
			myf.read(vec.data(), myf.frames());

			this->data() = MathUtil::deinterleave(vec, myf.channels());
		}
};
