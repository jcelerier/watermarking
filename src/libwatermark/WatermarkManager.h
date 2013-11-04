#pragma once
#include <memory>

#include "Parameters.h"
#include "watermark/watermarkbase.h"
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
		Input_p _input = nullptr;
		Output_p _output = nullptr;
		Watermark_p _watermark = nullptr;


		WatermarkManager(const Parameters<data_type>& parameters):
			conf(parameters)
		{

		}
		WatermarkManager(const WatermarkManager<data_type>& sm) = delete;
		const WatermarkManager& operator=(const WatermarkManager<data_type>& sm) = delete;

		void onDataUpdate()
		{
			_watermark->onDataUpdate();
		}

		// Algorithme principal (oui, toute cette complexité sert à avoir ce truc magnifiquement simple ici)
		void execute()
		{
			while(auto buf = _input->getNextBuffer())
			{
				(*_watermark)(buf);

				_output->writeNextBuffer(buf);
				delete buf;
			}
		}
};
