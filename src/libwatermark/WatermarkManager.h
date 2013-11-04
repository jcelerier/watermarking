#pragma once

#include "Parameters.h"
#include "watermark/WatermarkBase.h"
#include "io/InputManagerBase.h"
#include "io/OutputManagerBase.h"

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
		// Ajouter data : bits à encoder
		Input_p<data_type> input = nullptr;
		Output_p<data_type> output = nullptr;
		Watermark_p<data_type> algorithm = nullptr;


		WatermarkManager(const Parameters<data_type>& parameters):
			conf(parameters)
		{

		}
		WatermarkManager(const WatermarkManager<data_type>& sm) = delete;
		const WatermarkManager& operator=(const WatermarkManager<data_type>& sm) = delete;

		void onDataUpdate()
		{
			algorithm->onDataUpdate();
		}

		// Algorithme principal (oui, toute cette complexité sert à avoir ce truc magnifiquement simple ici)
		void execute()
		{
			while(Audio_p buf = input->getNextBuffer())
			{
				(*algorithm)(buf);
				output->writeNextBuffer(buf);
			}
		}
};
