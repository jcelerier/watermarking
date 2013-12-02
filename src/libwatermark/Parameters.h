#pragma once
#include <memory>
#include <complex>

#define DEBUG(x) std::cerr << #x << ": " << ( x ) << std::endl;
#ifndef M_PI
#define M_PI 3.141592653589793238462
#endif
template <typename data_type>
struct Parameters
{
		using size_type = long unsigned int;
		using complex_type = std::complex<data_type>;

		size_type samplingRate = 44100U;
		size_type bufferSize = 512U;

		data_type normFactor()
		{
			if(typeid(data_type) == typeid(double))
				return 1.0;
			if(typeid(data_type) == typeid(short))
				return std::pow(2, 15);
			return std::pow(2, 31);
		}
};
