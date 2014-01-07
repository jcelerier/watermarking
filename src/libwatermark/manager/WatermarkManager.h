#pragma once

#include "../watermark/WatermarkInterface.h"
#include "ManagerBase.h"
/**
 * @brief The WatermarkManager class
 *
 * Classe servant à appliquer une watermark à un signal audio.
 */
class WatermarkManager: public ManagerBase
{
	public:
		WatermarkManager(Input_p i,
						 Output_p o,
						 Watermark_p a,
						 WatermarkData_p d):
			ManagerBase(i, o),
			algorithm(a),
			data(d)
		{

		}

		WatermarkManager(const WatermarkManager& sm) = delete;
		WatermarkManager& operator=(const WatermarkManager& sm) = delete;

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
		virtual void execute() override
		{
			while(Audio_p buf = input->getNextBuffer())
			{
				timeAdapter->perform();

				if(timeAdapter->isRunning()) (*algorithm)(buf, *data.get());

				output->writeNextBuffer(buf);
			}
		}

	private:
		Watermark_p algorithm = nullptr;
		WatermarkData_p data = nullptr;
};
