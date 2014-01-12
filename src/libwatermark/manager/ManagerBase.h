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
		ManagerBase() = default;

		ManagerBase(Input_p i,
					Output_p o):
			m_input(i),
			m_output(o)
		{
		}

		ManagerBase(Input_p i,
					Output_p o,
					TimeAdapter_p t):
			m_input(i),
			m_output(o),
			m_timeAdapter(t)
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


		Input_p& input()
		{
			return m_input;
		}

		Output_p& output()
		{
			return m_output;
		}

		TimeAdapter_p& timeAdapter()
		{
			return m_timeAdapter;
		}

	protected:
		Input_p m_input = nullptr;
		Output_p m_output = nullptr;
		TimeAdapter_p m_timeAdapter = TimeAdapter_p(new AtTime);
};
