#pragma once

#include "BenchmarkBase.h"

template <typename data_type>
class FFTAmplify : public BenchmarkBase<data_type>
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;


	public:
		FFTAmplify(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>(configuration)
		{
			this->_name = "FFTAmplify";
		}

		virtual void operator()(Audio_p& data) override
		{
			// Recopier cette ligne
			auto& spectrum = static_cast<CData<typename Parameters<data_type>::complex_type>*>(data.get())->_data;

			for(auto& channel : spectrum)
			{
				for (auto& cplx : channel) // Dans l'ordre
				{
					double magnitude = std::abs(cplx) * _gain;
					double phase = std::arg(cplx);

					cplx = {magnitude * std::cos(phase), magnitude * std::sin(phase)};
				}
			}
		}

	private:
		double _gain = 0.1;
};
