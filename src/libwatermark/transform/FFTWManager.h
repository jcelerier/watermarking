#pragma once
#ifdef __APPLE__
#include "/usr/local/Cellar/fftw/3.3.3/include/fftw3.h"
#else
#include "fftw3.h"
#endif

#include "FFTManager.h"

template <typename data_type>
class FFTWManager : public FFTManager<data_type>
{
		using FFTManager<data_type>::_in;
		using FFTManager<data_type>::_out;
		using FFTManager<data_type>::_spectrum;
	public:
		using FFTManager<data_type>::FFTManager;

		FFTWManager(const FFTWManager<data_type>& orig) = delete;
		const FFTWManager<data_type>& operator=(const FFTWManager<data_type>& orig) = delete;

		virtual ~FFTWManager()
		{
			for(auto& plan : fw)
			{
				if(plan) fftw_destroy_plan(plan);
				plan = nullptr;
			}
			for(auto& plan : bw)
			{
				if(plan) fftw_destroy_plan(plan);
				plan = nullptr;
			}

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
			// Attention : FFTW_EXHAUSTIVE écrit dans les array d'entrée.
			for(auto i = 0U; i < _in.size(); ++i)
			{
				if(fw[i]) fftw_destroy_plan(fw[i]);
				fw[i] = fftw_plan_dft_r2c_1d((int)this->conf.bufferSize,
												  (double*)_in[i].data(),
												  reinterpret_cast<fftw_complex*>(_spectrum[i].data()),
												  FFTW_EXHAUSTIVE | FFTW_DESTROY_INPUT);

				if(bw[i]) fftw_destroy_plan(bw[i]);
				bw[i] = fftw_plan_dft_c2r_1d((int)this->conf.bufferSize,
												  reinterpret_cast<fftw_complex*>(_spectrum[i].data()),
												  (double*)_out[i].data(),
												  FFTW_EXHAUSTIVE | FFTW_DESTROY_INPUT);
			}
		}

	private:
		std::vector<fftw_plan> fw = {};
		std::vector<fftw_plan> bw = {};

};
