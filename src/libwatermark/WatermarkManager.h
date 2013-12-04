#pragma once

#include "Parameters.h"
#include "watermark/WatermarkBase.h"
#include "io/InputManagerBase.h"
#include "io/OutputManagerBase.h"
#include "timeadapter/TimeAdapter.h"
#include "timeadapter/AtTime.h"
/**
 * @brief Main class.
 *
 */
template<typename data_type>
class WatermarkManager
{
	private:
		const Parameters<data_type>& conf;

	public:
		Input_p<data_type> input = nullptr;
		Output_p<data_type> output = nullptr;
		Watermark_p<data_type> algorithm = nullptr;
		WatermarkData_p data = nullptr;
		TimeAdapter_p timeAdapter = nullptr;


		WatermarkManager(const Parameters<data_type>& parameters):
			conf(parameters),
			timeAdapter(new AtTime)
		{

		}

		WatermarkManager(const WatermarkManager<data_type>& sm) = delete;
		const WatermarkManager& operator=(const WatermarkManager<data_type>& sm) = delete;

		void onDataUpdate()
		{
			algorithm->onDataUpdate();
		}

		void prepare()
		{
			timeAdapter->addHandler(std::bind(&WatermarkData::resetPosition, data));
		}

		// Algorithme principal (oui, toute cette complexité sert à avoir ce truc magnifiquement simple ici)
		void execute()
		{
			while(Audio_p buf = input->getNextBuffer())
			{
				if(timeAdapter->check())
					(*algorithm)(buf, *data.get());

				output->writeNextBuffer(buf);
				timeAdapter->increment();
			}
		}
};
