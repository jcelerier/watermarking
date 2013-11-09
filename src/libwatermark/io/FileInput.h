#pragma once
#include <istream>
#include <fstream>

#include <sndfile.hh>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class FileInput : public InputManagerBase<data_type>
{
		using IOManagerBase<data_type>::v;

	public:
		FileInput(std::string filename, const Parameters<data_type>& cfg):
			InputManagerBase<data_type>(cfg)
		{
			readFile(filename.c_str());
		}

		FileInput(std::string filename, InputCopy<data_type>* icp, const Parameters<data_type>& cfg):
					InputManagerBase<data_type>(icp, cfg)
		{
			readFile(filename.c_str());
		}

		void readFile(const char * str)
		{
			auto myf = SndfileHandle(str);

			std::vector<data_type> vec;
			vec.resize(myf.frames() * myf.channels());

			for(auto i = 0U; i < myf.channels(); ++i)
				myf.read(vec.data() + i * (myf.frames() - 1),  myf.frames() - 1);

			v() = MathUtil::deinterleave(vec, (unsigned int) myf.channels(), (unsigned int) myf.frames());
		}
};
