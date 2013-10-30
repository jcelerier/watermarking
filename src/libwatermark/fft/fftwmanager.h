#pragma once
#include <fftw3.h>

#include "fftmanager.h"


/**
 * @brief The FFTWManager class
 *
 * Implementation of the FFTW process.
 */
template <typename data_type>
class FFTWManager : public FFTManager<data_type>
{
	public:
		FFTWManager(const Parameters<data_type>& conf):
			FFTManager<data_type>(conf)
		{
			updateSize();
		}


		virtual ~FFTWManager()
		{
			if(plan_fw) fftw_destroy_plan(plan_fw);
			if(plan_bw) fftw_destroy_plan(plan_bw);

			fftw_cleanup();
		}
		virtual FFTManager<data_type>* clone() override
		{
			return new FFTWManager<data_type>(*this);
		}

		virtual void forward() const override
		{
			fftw_execute(plan_fw);
		}
		virtual void backward() const override
		{
			fftw_execute(plan_bw);
		}

		virtual double normalizationFactor() const override
		{
			return 1.0 / FFTManager<data_type>::conf.bufferSize;
		}

		virtual void updateSize() override
		{
			if(plan_fw) fftw_destroy_plan(plan_fw);
			if(plan_bw) fftw_destroy_plan(plan_bw);

			// Initialize the fftw plans
			plan_fw = fftw_plan_dft_r2c_1d(FFTManager<data_type>::conf.bufferSize,
										   FFTManager<data_type>::_in.data(),
										   reinterpret_cast<fftw_complex*>(FFTManager<data_type>::_spectrum.data()),
										   FFTW_ESTIMATE);
			plan_bw = fftw_plan_dft_c2r_1d(FFTManager<data_type>::conf.bufferSize,
										   reinterpret_cast<fftw_complex*>(FFTManager<data_type>::_spectrum.data()),
										   FFTManager<data_type>::_out.data(),
										   FFTW_ESTIMATE);
		}

	private:
		fftw_plan plan_fw = nullptr; /**< TODO */
		fftw_plan plan_bw = nullptr; /**< TODO */
};


