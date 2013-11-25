#pragma once
#include <vector>
#include <stdint.h>
#include <bitset>
#include <algorithm>
#include <iostream>
class WatermarkData
{
	public:
		uint64_t _size = 0;
		std::vector<bool> bits = { };
		unsigned int position = 0;
		WatermarkData() = default;

		void setSize(const uint64_t size)
		{
			const auto size_bits = sizeof(_size) * 8U;
			_size = size + size_bits;
			std::bitset<size_bits> num(_size);

			for(auto i = 0U; i < size_bits; ++i)
				bits.push_back(num[i]);
		}

		uint64_t readSizeFromBits()
		{
			const auto size_bits = sizeof(_size) * 8U;
			std::bitset<size_bits> num;

			for(auto i = 0U; i < size_bits; ++i)
				num[i] = bits[i];

			return _size = num.to_ullong();
		}

		void printBits()
		{
			std::cerr << "Affichage des données :" << std::endl;
			const auto size_bits = sizeof(_size) * 8U;
			for(auto i = size_bits; i < _size; ++i)
			{
				std::cerr << bits[i];
			}

			std::cerr << std::endl;
		}
};


using WatermarkData_p = std::unique_ptr<WatermarkData>;
