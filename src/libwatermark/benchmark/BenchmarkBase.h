#pragma once

#include "../Parameters.h"
#include "../Data.h"


template <typename data_type>
/**
 * @brief The BenchmarkBase class
 *
 * Classe de base pour des benchmarks / effets audio.
 */
class BenchmarkBase : public BenchmarkInterface
{
	public:
		using size_type = typename Parameters<data_type>::size_type;
		BenchmarkBase(const Parameters<data_type>& configuration):
			conf(configuration)
		{
		}

		virtual ~BenchmarkBase() = default;

	protected:
		const Parameters<data_type>& conf;
};

