#pragma once
#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"
#include <ostream>
#include <fstream>
#include <algorithm>

class FileOutput : public OutputManagerBase
{
	public:
		FileOutput(const Parameters& cfg):
			OutputManagerBase(cfg)
		{

		}

		void writeFile(const char * str)
		{
			std::ofstream ofile(str, std::ios_base::ate | std::ios_base::binary);

			short* buff = new short[_baseData.size()];
			std::transform(_baseData.begin(), _baseData.end(), buff, [] (data_type sample) { return MathUtil::DoubleToShort(sample); });

			ofile.write((char*)&buff, _baseData.size());
			delete buff;

			ofile.close();
		}
};
