#pragma once

#include "ManagerBase.h"
#include "../benchmark/BenchmarkInterface.h"

/**
 * @brief Main class.
 *
 */
class BenchmarkManager: public ManagerBase
{
	public:
		BenchmarkManager(Input_p i,
						 Output_p o,
						 Benchmark_p a):
			ManagerBase(i, o),
			algorithm(a)
		{
		}

		BenchmarkManager(const BenchmarkManager& sm) = delete;
		BenchmarkManager& operator=(const BenchmarkManager& sm) = delete;

		// Algorithme principal (oui, toute cette complexité sert à avoir ce truc magnifiquement simple ici)
		virtual void execute() override
		{
			while(Audio_p buf = m_input->getNextBuffer())
			{
				m_timeAdapter->perform();

				if(m_timeAdapter->isRunning()) (*algorithm)(buf);

				m_output->writeNextBuffer(buf);
			}
		}

	private:
		Benchmark_p algorithm = nullptr;
};
