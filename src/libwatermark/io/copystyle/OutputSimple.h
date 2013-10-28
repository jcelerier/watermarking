#pragma once
#include "output.h"

class OutputSimple
{
	public:
		virtual void copy(std::vector<data_type>::const_iterator in,
						  std::vector<data_type>::iterator out,
						  size_type pos,
						  size_type in_length,
						  size_type out_length) final override
		{
			auto normalizeFFT = [&](double x) { return x * _fft->normalizationFactor(); };
			if (_fft->size() <= _tabLength - pos)
			{
				std::transform(_fft->output(), _fft->output() + _fft->size(), _data + pos, normalizeFFT);
			}
			else //fileSize - pos < fftSize
			{
				std::transform(_fft->output(), _fft->output() + _tabLength - pos, _data + pos, normalizeFFT);
			}
		}
};
