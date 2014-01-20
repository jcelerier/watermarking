#pragma once
#include <algorithm>

#include "BenchmarkBase.h"
#include "properties/Amplitude.h"
#include "properties/Frequency.h"

template <typename data_type>
class AddBrumm : public BenchmarkBase<data_type>,
		public AmplitudeProperty,
		public FrequencyProperty
{
		using BenchmarkBase<data_type>::conf;
		using size_type = typename Parameters<data_type>::size_type;

	public:
        AddBrumm(const Parameters<data_type>& configuration):
			BenchmarkBase<data_type>("AddBrumm", configuration)
		{
		}

		virtual void operator()(Audio_p& data) override
		{
			auto& channelsData = getAudio<data_type>(data);

			double const cst = 2.0 * M_PI * _frequency / conf.samplingRate;

			// Pour chaque canal (gauche - droite)
			for(auto& sampleData : channelsData)
			{
				// Pour chaque échantillon (dans l'ordre)
				for(auto& sample : sampleData)
				{
					sample =  sample + _amplitude * sin(cst * pos + dist(rng));
					pos = (pos == (conf.samplingRate / _frequency) - 1) ? 0 : pos + 1;
				}
			}
		}

	private:
		size_type pos = 0;

		std::default_random_engine rng = std::default_random_engine(std::random_device{}());
		std::uniform_real_distribution<double> dist = std::uniform_real_distribution<double>(0, 0.1);

};
