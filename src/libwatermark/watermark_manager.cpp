#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include "watermark_manager.h"
#include "watermark/algorithms.h"
#include "mathutils/math_util.h"
#include "fft/fftwmanager.h"

//TODO Passer la FFT en proxy.

WatermarkManager::WatermarkManager(const unsigned int fft_Size, const unsigned int sampling_Rate):
	_samplingRate(sampling_Rate),
	_fft(new FFTWManager)
{
	_fft->updateSize(fft_Size);
	onFFTSizeUpdate();
}

WatermarkManager::WatermarkManager(const WatermarkManager &sm):
	_dataSource(sm._dataSource),
	_samplingRate(sm.getSamplingRate()),
	_fft(nullptr),
	_watermark(nullptr)

{
	_fft.reset(sm._fft->clone());
	_watermark.reset(sm._watermark->clone());

	_fft->updateSize(sm._fft->size());
	onFFTSizeUpdate();
}

const WatermarkManager &WatermarkManager::operator=(const WatermarkManager &sm)
{
	_dataSource = sm._dataSource;
	_samplingRate = sm.getSamplingRate();

	_fft.reset(sm._fft->clone());
	_watermark.reset(sm._watermark->clone());

	_fft->updateSize(sm._fft->size());
	onFFTSizeUpdate();
	return *this;
}



void WatermarkManager::execute()
{
	// Some configuration and cleaning according to the parameters used
	if (bypass()) return;
	if (dataSource() == DataSource::File)
	{
		initDataArray();
	}

	SpectralWatermarkBase* specWatermark = dynamic_cast<SpectralWatermarkBase*>(_watermark.get());

	//
//	while(double* buf = _data_in.getNextBuffer())
//	{
//		(*watermark(buf, data_to_encode));

//		_data_out.setNextBuffer(buf);
//		delete buf;
//	}
	// Execution of the spectral algorithm
	for (auto sample_n = 0U; sample_n < getLength(); sample_n += getFrameIncrement())
	{
		copyInput(sample_n);
		_fft->forward();

		if(dataSource() == DataSource::File && sample_n == 0)
			onDataUpdate();

		// Spectral subtraction
		(*specWatermark)(_fft->spectrum());

		_fft->backward();
		copyOutput(sample_n);
	}
}



void WatermarkManager::onFFTSizeUpdate()
{
	_ola_frame_increment = _fft->size() / 2;
	_std_frame_increment = _fft->size();

	SpectralWatermarkBase* specWatermark = dynamic_cast<SpectralWatermarkBase*>(_watermark.get());
	if(specWatermark) specWatermark->onFFTSizeUpdate();
}



WatermarkManager::~WatermarkManager()
{
	_watermark.reset();
}


void WatermarkManager::onDataUpdate()
{
	_watermark->onDataUpdate();
}

void WatermarkManager::writeBuffer(short * const buffer) const
{
	std::transform(_data, _data + _tabLength, buffer, MathUtil::DoubleToShort);
}

void WatermarkManager::copyOutputSimple(const unsigned int pos)
{
}

void WatermarkManager::copyInputOLA(const unsigned int pos)
{
}

void WatermarkManager::copyOutputOLA(const unsigned int pos)
{
}



WatermarkManager::DataSource WatermarkManager::dataSource() const
{
	return _dataSource;
}



unsigned int WatermarkManager::getFrameIncrement() const
{
	return _useOLA? _ola_frame_increment : _std_frame_increment;
}

WatermarkBase* WatermarkManager::getWatermarkImplementation() const
{
	return _watermark.get();
}

void WatermarkManager::setWatermarkImplementation(WatermarkBase *value)
{
	_watermark.reset(value);
	onFFTSizeUpdate();
}

unsigned int WatermarkManager::getSamplingRate() const
{
	return _samplingRate;
}

void WatermarkManager::setSamplingRate(const unsigned int value)
{
	_samplingRate = value;
	onFFTSizeUpdate();
}

void WatermarkManager::readParametersFromFile()
{
	enum Algorithm { SSWBasic, Bypass};
	static const std::map<std::string, Algorithm> algo
	{
		std::make_pair("sswbasic", Algorithm::SSWBasic),
		std::make_pair("bypass", Algorithm::Bypass)
	};

//	std::ifstream f("subtraction.conf");
//	double alpha, beta, alphawt, betawt;
//	unsigned int iterations_tmp;
	std::string noise_alg, alg;

	// Class members
	//f >> ...;

//	f.close();

	if (algo.find(alg) != algo.end())
	{
		switch(algo.at(alg))
		{
			case Algorithm::Bypass:
			{
				// Check for memory leak.
				BypassWatermark* wmrk = new BypassWatermark(*this);
				setWatermarkImplementation(wmrk);
				break;
			}
		}
	}
	else
	{
		std::cerr << "Invalid subtraction algorithm";
	}
}

unsigned int WatermarkManager::FFTSize() const
{
	return _fft->size();
}

void WatermarkManager::setFftSize(unsigned int value)
{
	_fft->updateSize(value);

	onFFTSizeUpdate();
}

unsigned int WatermarkManager::spectrumSize() const
{
	return _fft->spectrumSize();
}
