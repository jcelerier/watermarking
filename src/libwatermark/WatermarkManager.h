#pragma once

#include "Parameters.h"
#include "watermark/WatermarkBase.h"
#include "io/InputManagerBase.h"
#include "io/OutputManagerBase.h"
#include "timeadapter/TimeAdapter.h"
#include "timeadapter/AtTime.h"


/**
 * @brief The WatermarkManager class
 *
 * Classe servant à appliquer une watermark à un signal audio.
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

		/**
		 * @brief prepare Prépare les données nécessaires
		 *
		 * A appeler juste avant execute() de l'algorithme.
		 */
		void prepare()
		{
			timeAdapter->addStartHandler(std::bind(&WatermarkData::resetPosition, data));
		}

		/**
		 * @brief execute
		 *
		 * Méthode principale qui va traiter tout l'audio.
		 */
		void execute()
		{
			while(Audio_p buf = input->getNextBuffer())
			{
				timeAdapter->perform();

				if(timeAdapter->isRunning()) (*algorithm)(buf, *data.get());

				output->writeNextBuffer(buf);
			}
		}
};
