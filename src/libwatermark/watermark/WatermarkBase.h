#pragma once

#include "../Parameters.h"
#include "../Data.h"


template <typename data_type>
class WatermarkBase
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		WatermarkBase(const Parameters<data_type>& configuration):
			conf(configuration)
		{
		}

		virtual void operator()(Audio_p& data) = 0;
		virtual void onDataUpdate() = 0;
		virtual ~WatermarkBase() = default;

	protected:
		const Parameters<data_type>& conf;
};

template<typename T>
using Watermark_p = std::shared_ptr<WatermarkBase<T>>;
