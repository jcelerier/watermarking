#pragma once
#include <fftw3.h>

#include "fftmanager.h"


/**
 * @brief The FFTWManager class
 *
 * Implementation of the FFTW process.
 */
class FFTWManager : public FFTManager
{
	public:
		FFTWManager(const Parameters& conf):
			FFTManager(conf)
		{
			updateSize();
		}


		virtual ~FFTWManager()
		{
			if(plan_fw) fftw_destroy_plan(plan_fw);
			if(plan_bw) fftw_destroy_plan(plan_bw);

			fftw_cleanup();
		}
		virtual FFTManager* clone() override
		{
			return new FFTWManager(*this);
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
			return 1.0 / conf.bufferSize;
		}

		virtual void updateSize() override
		{
			if(plan_fw) fftw_destroy_plan(plan_fw);
			if(plan_bw) fftw_destroy_plan(plan_bw);

			// Initialize the fftw plans
			plan_fw = fftw_plan_dft_r2c_1d(conf.bufferSize, _in.data(), reinterpret_cast<fftw_complex*>(&_spectrum[0]),  FFTW_ESTIMATE);
			plan_bw = fftw_plan_dft_c2r_1d(conf.bufferSize, reinterpret_cast<fftw_complex*>(&_spectrum[0]), _out.data(), FFTW_ESTIMATE);
		}
	private:
		fftw_plan plan_fw = nullptr; /**< TODO */
		fftw_plan plan_bw = nullptr; /**< TODO */
};


