#pragma once
#include <vector>
#include <stdint.h>

class WatermarkData
{
	public:
		uint64_t _size;
		std::vector<bool> bits;
		unsigned int position = 0;
		WatermarkData() = default;

		void setSize(uint64_t size)
		{
			_size = size;
			for(int index = 0; index<sizeof(size); index++)
			{
				uint64_t bit64 = 0x00000001;
				bool bit;
				bit = size & (0x000000001 >> index);
				bits.push_back(bit);
				position++;
			}
		}
};


using WatermarkData_p = std::unique_ptr<WatermarkData>;
