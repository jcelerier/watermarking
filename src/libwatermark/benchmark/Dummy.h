#pragma once

#include "BenchmarkBase.h"

template <typename data_type>
class Dummy : public BenchmarkBase<data_type>
{
	public:
		Dummy(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("Dummy", configuration)
		{
		}

		virtual void operator()(Audio_p& ) override
		{
		}
};
