#pragma once

#include "../Parameters.h"
#include "../Data.h"


template <typename data_type>
class BenchmarkBase
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		BenchmarkBase(const Parameters<data_type>& configuration):
			conf(configuration)
		{
		}

		virtual void operator()(Audio_p& data) = 0;
		virtual ~BenchmarkBase() = default;

	protected:
		const Parameters<data_type>& conf;
};

template<typename T>
using Benchmark_p = std::shared_ptr<BenchmarkBase<T>>;
