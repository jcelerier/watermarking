#pragma once
#include <fftw3.h>

#include "fftmanager.h"

template <typename data_type>
class FFTWManager : public FFTManager<data_type>
{
	public:
		FFTWManager(const Parameters<data_type>& cfg):
			FFTManager<data_type>(cfg)
		{
			num_instances++;
		}

		FFTWManager(const FFTWManager<data_type>& orig) = delete;
		const FFTWManager<data_type>& operator=(const FFTWManager<data_type>& orig) = delete;

		virtual ~FFTWManager()
		{
			for(auto& plan : fw)
				if(plan) fftw_destroy_plan(plan);

			for(auto& plan : bw)
				if(plan) fftw_destroy_plan(plan);

			if(!num_instances--)
				fftw_cleanup();
		}

		virtual void setChannels(unsigned int n) override
		{
			FFTManager<data_type>::setChannels(n);
			fw.resize(n);
			bw.resize(n);

			updateSize();

		}

		virtual void forward() const override
		{
			for(auto& plan : fw)
				fftw_execute(plan);
		}

		virtual void backward() const override
		{
			for(auto& plan : bw)
				fftw_execute(plan);
		}

		virtual double normalizationFactor() const override
		{
			return 1.0 / this->conf.bufferSize;
		}

		virtual void updateSize() override
		{
			// channels
			for(auto i = 0U; i < this->_in.size(); ++i)
			{
				if(fw[i]) fftw_destroy_plan(fw[i]);
				fw[i] = fftw_plan_dft_r2c_1d((int)this->conf.bufferSize,
												  this->_in[i].data(),
												  reinterpret_cast<fftw_complex*>(this->_spectrum[i].data()),
												  FFTW_EXHAUSTIVE);

				if(bw[i]) fftw_destroy_plan(bw[i]);
				bw[i] = fftw_plan_dft_c2r_1d((int)this->conf.bufferSize,
												  reinterpret_cast<fftw_complex*>(this->_spectrum[i].data()),
												  this->_out[i].data(),
												  FFTW_EXHAUSTIVE);
			}
		}

	private:
		std::vector<fftw_plan> fw = {};
		std::vector<fftw_plan> bw = {};

		static unsigned int num_instances;
};

template<>
unsigned int FFTWManager<double>::num_instances = 0;
