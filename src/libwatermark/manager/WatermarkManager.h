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
		WatermarkManager(WatermarkData_p d):
			m_data(d)
		{

		}

		WatermarkManager(Input_p i,
						 Output_p o,
						 Watermark_p a,
						 WatermarkData_p d):
			ManagerBase(i, o),
			m_algorithm(a),
			m_data(d)
		{

		}

		WatermarkManager(Input_p i,
						 Output_p o,
						 TimeAdapter_p t,
						 Watermark_p a,
						 WatermarkData_p d):
			ManagerBase(i, o, t),
			m_algorithm(a),
			m_data(d)
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
			m_timeAdapter->addStartHandler(std::bind(&WatermarkData::resetPosition, m_data));
		}

		/**
		 * @brief execute
		 *
		 * Méthode principale qui va traiter tout l'audio.
		 */
		virtual void execute() override
		{
			while(Audio_p buf = m_input->getNextBuffer())
			{
				m_timeAdapter->perform();

				if(m_timeAdapter->isRunning()) (*m_algorithm)(buf, *m_data.get());

				m_output->writeNextBuffer(buf);
			}
			m_algorithm->finish(*m_data.get());
		}

		Watermark_p& algorithm()
		{
			return m_algorithm;
		}

	private:
		Watermark_p m_algorithm = nullptr;
		WatermarkData_p m_data = nullptr;
};
