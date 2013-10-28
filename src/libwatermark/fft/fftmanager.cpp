#include "fftmanager.h"
#include <algorithm>

FFTManager::FFTManager(const Parameters & config):
	conf(config)
{
}

FFTManager::FFTManager(const FFTManager &fm):
	conf(fm.conf)
{
	_in = new data_type[conf.bufferSize];
	_out = new data_type[conf.bufferSize];
	_spectrum = new std::complex<data_type>[spectrumSize()];

	std::copy_n(fm.input(), conf.bufferSize, _in);
	std::copy_n(fm.output(), conf.bufferSize, _out);
	std::copy_n(fm.spectrum(), spectrumSize(), _spectrum);
}

const FFTManager &FFTManager::operator=(const FFTManager &fm)
{
	std::copy_n(fm.input(), conf.bufferSize, _in);
	std::copy_n(fm.output(), conf.bufferSize, _out);
	std::copy_n(fm.spectrum(), spectrumSize(), _spectrum);

	return *this;
}

FFTManager::~FFTManager()
{
	delete[] _in;
	delete[] _out;
	delete[] _spectrum;
}


FFTManager::data_type *FFTManager::input() const
{
	return _in;
}

FFTManager::data_type* FFTManager::output() const
{
	return _out;
}

std::complex<FFTManager::data_type> *FFTManager::spectrum() const
{
	return _spectrum;
}

FFTManager::size_type FFTManager::spectrumSize() const
{
	return conf.bufferSize / 2 + 1;
}

//void FFTManager::updateSize(const size_type n)
//{
//	_fftSize = n;
//	_in = new data_type[conf.bufferSize];
//	_out = new data_type[conf.bufferSize];
//	_spectrum = new std::complex<data_type>[spectrumSize()];
//}
