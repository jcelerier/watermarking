#pragma once
#include "../Parameters.h"
#include "../Data.h"

class WatermarkBase
{
	public:
		using size_type = Parameters::size_type;
		using data_type = Parameters::data_type;
		WatermarkBase(const Parameters& configuration):
			conf(configuration)
		{
		}

		virtual ~WatermarkBase() = default;
		virtual WatermarkBase* clone() = 0;

		virtual void operator()(IData* const) = 0;

		virtual void onDataUpdate() = 0;

	protected:
		const Parameters& conf;
};
