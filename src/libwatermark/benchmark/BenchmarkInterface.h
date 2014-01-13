#pragma once

#include "../Data.h"
#include <string>

class BenchmarkInterface
{
	public:
		virtual ~BenchmarkInterface() = default;
		virtual void operator()(Audio_p& data) = 0;

		std::string _name = "";
};

using Benchmark_p = std::shared_ptr<BenchmarkInterface>;
