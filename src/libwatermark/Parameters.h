#pragma once

struct Parameters
{
		using data_type = double;
		using size_type = long unsigned int;

		size_type samplingRate = 44100;
		size_type bufferSize = 512;
};
