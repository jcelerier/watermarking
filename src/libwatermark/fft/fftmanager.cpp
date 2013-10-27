#include "fftmanager.h"
#include <algorithm>

FFTManager::FFTManager()
{
}

FFTManager::FFTManager(const FFTManager &fm)
{
	_fftSize = fm.size();
	_in = new double[size()];
	_out = new double[size()];
	_spectrum = new std::complex<double>[spectrumSize()];

	std::copy_n(fm.input(), size(), _in);
	std::copy_n(fm.output(), size(), _out);
	std::copy_n(fm.spectrum(), spectrumSize(), _spectrum);
}

const FFTManager &FFTManager::operator=(const FFTManager &fm)
{
	if(_fftSize != fm.size())
	{
		_fftSize = fm.size();
		delete[] _in;
		delete[] _out;
		delete[] _spectrum;
		_in = new double[size()];
		_out = new double[size()];
		_spectrum = new std::complex<double>[spectrumSize()];
	}

	std::copy_n(fm.input(), size(), _in);
	std::copy_n(fm.output(), size(), _out);
	std::copy_n(fm.spectrum(), spectrumSize(), _spectrum);

	return *this;
}

FFTManager::~FFTManager()
{
	delete[] _in;
	delete[] _out;
	delete[] _spectrum;
}


double *FFTManager::input() const
{
	return _in;
}

double *FFTManager::output() const
{
	return _out;
}

std::complex<double> *FFTManager::spectrum() const
{
	return _spectrum;
}

unsigned int FFTManager::spectrumSize() const
{
	return _fftSize / 2 + 1;
}

unsigned int FFTManager::size() const
{
	return _fftSize;
}

void FFTManager::updateSize(const unsigned int n)
{
	_fftSize = n;
	_in = new double[size()];
	_out = new double[size()];
	_spectrum = new std::complex<double>[spectrumSize()];
}
