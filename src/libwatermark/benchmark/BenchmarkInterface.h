#pragma once

#include "../Data.h"
#include <string>

class BenchmarkInterface
{
	public:
		BenchmarkInterface(const std::string name):
			_name(name)
		{
		}

		virtual ~BenchmarkInterface() = default;
		virtual void operator()(Audio_p& data) = 0;

		std::string _name = "";
};

using Benchmark_p = std::shared_ptr<BenchmarkInterface>;
