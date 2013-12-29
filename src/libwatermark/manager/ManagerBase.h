#pragma once

#include "../Parameters.h"
#include "../watermark/WatermarkBase.h"
#include "../io/IOInterface.h"
#include "../timeadapter/AtTime.h"


/**
 * @brief The ManagerBase class
 *
 * Classe servant à appliquer une watermark à un signal audio.
 */
class ManagerBase
{
	public:
		ManagerBase(Input_p i,
					Output_p o):
			input(i),
			output(o),
			timeAdapter(new AtTime)
		{
		}

		ManagerBase(Input_p i,
					Output_p o,
					TimeAdapter_p t):
			input(i),
			output(o),
			timeAdapter(t)
		{
		}

		ManagerBase(const ManagerBase& sm) = delete;
		ManagerBase& operator=(const ManagerBase& sm) = delete;

		/**
		 * @brief prepare Prépare les données nécessaires
		 *
		 * A appeler juste avant execute() de l'algorithme.
		 */
		virtual void prepare()
		{
		}

		/**
		 * @brief execute
		 *
		 * Méthode principale qui va traiter tout l'audio.
		 */
		virtual void execute() = 0;

	protected:
		Input_p input = nullptr;
		Output_p output = nullptr;
		TimeAdapter_p timeAdapter = nullptr;
};
