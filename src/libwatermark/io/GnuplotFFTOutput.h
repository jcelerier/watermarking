#pragma once

#include "OutputManagerBase.h"
#include "gnuplot/gnuplot_i.h"


template <typename data_type>
class GnuplotFFTOutput:public OutputManagerBase<data_type>
{
	private:
		gnuplot_ctrl *h = nullptr;
		Output_p<data_type> outputImpl = nullptr;

	public:
		GnuplotFFTOutput(OutputManagerBase<data_type>* output, const Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(nullptr, cfg),
			outputImpl(output)
		{
			h = gnuplot_init();
			gnuplot_setstyle(h, "lines");
		}

		virtual ~GnuplotFFTOutput()
		{
			gnuplot_close(h);
		}

		GnuplotFFTOutput(const GnuplotFFTOutput&) = delete;
		const GnuplotFFTOutput& operator=(const GnuplotFFTOutput&) = delete;


		virtual void writeNextBuffer(Audio_p& abstract_buffer) override
		{
			auto& buffer = static_cast<CData<typename Parameters<data_type>::complex_type>*>(abstract_buffer.get())->_data;

			for(auto& channel : buffer)
			{
				std::vector<data_type> power(channel.size());
				std::vector<data_type> phase(channel.size());
				MathUtil::computePowerAndPhaseSpectrum(channel.data(), power.data(), phase.data(), channel.size());
				gnuplot_resetplot(h);
				gnuplot_plot_x(h, power.data(), power.size(), "Plot");
				sleep(3);
			}
			outputImpl->writeNextBuffer(abstract_buffer);
		}
};
