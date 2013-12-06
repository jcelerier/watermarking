#pragma once

#include "Parameters.h"
#include "benchmark/BenchmarkBase.h"
#include "io/InputManagerBase.h"
#include "io/OutputManagerBase.h"
#include "timeadapter/TimeAdapter.h"
#include "timeadapter/AtTime.h"
#include "timeadapter/Every_For.h"

/**
 * @brief Main class.
 *
 */
template<typename data_type>
class BenchmarkManager
{
	private:
		const Parameters<data_type>& conf;

	public:
		// Ajouter data : bits à encoder
		Input_p<data_type> input = nullptr;
		Output_p<data_type> output = nullptr;
		Benchmark_p<data_type> algorithm = nullptr;
		TimeAdapter_p timeAdapter = nullptr;


		BenchmarkManager(const Parameters<data_type>& parameters):
			conf(parameters),
			timeAdapter(new AtTime)
			//timeAdapter(new Every_For(400, 200, 30))
		{

		}

		void prepare()
		{

		}

		BenchmarkManager(const BenchmarkManager<data_type>& sm) = delete;
		const BenchmarkManager& operator=(const BenchmarkManager<data_type>& sm) = delete;

		// Algorithme principal (oui, toute cette complexité sert à avoir ce truc magnifiquement simple ici)
		void execute()
		{
			while(Audio_p buf = input->getNextBuffer())
			{
				timeAdapter->perform();

				if(timeAdapter->isRunning()) (*algorithm)(buf);

				output->writeNextBuffer(buf);
			}
		}
};
