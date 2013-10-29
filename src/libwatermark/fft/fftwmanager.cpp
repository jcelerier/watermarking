#include "fftwmanager.h"


unsigned int FFTWManager::_num_instances = 0;

FFTWManager::FFTWManager(const Parameters& config):
	FFTManager(config)
{
	_num_instances++;
	updateSize();
}

FFTWManager::~FFTWManager()
{
	if(plan_fw) fftw_destroy_plan(plan_fw);
	if(plan_bw) fftw_destroy_plan(plan_bw);

	_num_instances--;
	if(_num_instances == 0)
		fftw_cleanup();
}

FFTManager *FFTWManager::clone()
{
	return new FFTWManager(*this);
}

void FFTWManager::forward() const
{
	fftw_execute(plan_fw);
}

void FFTWManager::backward() const
{
	fftw_execute(plan_bw);
}

double FFTWManager::normalizationFactor() const
{
	return 1.0 / conf.bufferSize;
}

void FFTWManager::updateSize()
{
	if(plan_fw) fftw_destroy_plan(plan_fw);
	if(plan_bw) fftw_destroy_plan(plan_bw);

	// Initialize the fftw plans
	plan_fw = fftw_plan_dft_r2c_1d(conf.bufferSize, _in.data(), reinterpret_cast<fftw_complex*>(&_spectrum[0]),  FFTW_ESTIMATE);
	plan_bw = fftw_plan_dft_c2r_1d(conf.bufferSize, reinterpret_cast<fftw_complex*>(&_spectrum[0]), _out.data(), FFTW_ESTIMATE);
}

