#pragma once
class Parameters;

class WatermarkBase
{
	public:
		WatermarkBase(const Parameters& configuration):
			conf(configuration)
		{
		}

		virtual ~WatermarkBase() = default;
		virtual WatermarkBase* clone() = 0;

		/**
		 * @brief Actions to perform if the data changes a lot.
		 *
		 * For instance, discard previous data saved by the algorithms which would not have sense anymore.
		 */
		virtual void onDataUpdate() = 0;

	protected:
		const Parameters& conf;
};
