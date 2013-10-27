#pragma once

#include "fftmanager.h"
#include <fftw3.h>

/**
 * @brief The FFTWManager class
 *
 * Implementation of the FFTW process.
 */
class FFTWManager : public FFTManager
{
	public:
		FFTWManager();
		FFTWManager(const FFTWManager& fm);
		const FFTWManager &operator=(const FFTWManager& fm);

		virtual ~FFTWManager();
		virtual FFTManager* clone() override;

		virtual void forward() const override;
		virtual void backward() const override;

		virtual void updateSize(const unsigned int) override;
		virtual double normalizationFactor() const override;

	private:
		fftw_plan plan_fw = nullptr; /**< TODO */
		fftw_plan plan_bw = nullptr; /**< TODO */

		static unsigned int _num_instances;
};

