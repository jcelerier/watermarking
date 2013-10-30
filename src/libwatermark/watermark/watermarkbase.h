#pragma once

#include "../Parameters.h"
#include "../Data.h"

class IWatermark
{
	public:
		virtual ~IWatermark() = default;
		virtual IWatermark* clone() = 0;
		virtual void operator()(IData* const) = 0;
		virtual void onDataUpdate() = 0;
};


template <typename data_type>
class WatermarkBase: public IWatermark
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		WatermarkBase(const Parameters<data_type>& configuration):
			conf(configuration)
		{
		}

		virtual ~WatermarkBase() = default;

	protected:
		const Parameters<data_type>& conf;
};


typedef std::shared_ptr<IWatermark> Watermark_p;
