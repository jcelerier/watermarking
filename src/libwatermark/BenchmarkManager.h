#pragma once

#include "Parameters.h"
#include "benchmark/BenchmarkBase.h"
#include "io/InputManagerBase.h"
#include "io/OutputManagerBase.h"

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


		BenchmarkManager(const Parameters<data_type>& parameters):
			conf(parameters)
		{

		}

		BenchmarkManager(const BenchmarkManager<data_type>& sm) = delete;
		const BenchmarkManager& operator=(const BenchmarkManager<data_type>& sm) = delete;

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
