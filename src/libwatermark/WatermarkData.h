#pragma once
#include <vector>

class WatermarkData
{
	public:
		std::vector<bool> bits;
		unsigned int position = 0;
		WatermarkData() = default;
};


using WatermarkData_p = std::unique_ptr<WatermarkData>;
