#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include "watermark_manager.h"
#include "watermark/algorithms.h"
#include "mathutils/math_util.h"
#include "fft/fftwmanager.h"


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
	_watermark(nullptr),
	_tabLength(sm._tabLength),
	_data(new double[_tabLength]),
	_origData(new double[_tabLength]),
	_useOLA(sm._useOLA)

{
	_fft.reset(sm._fft->clone());
	_watermark.reset(sm._watermark->clone());

	_fft->updateSize(sm._fft->size());
	onFFTSizeUpdate();
	std::copy_n(sm._data, _tabLength, _data);
	std::copy_n(sm._origData, _tabLength, _origData);
}

const WatermarkManager &WatermarkManager::operator=(const WatermarkManager &sm)
{
	_dataSource = sm._dataSource;
	_samplingRate = sm.getSamplingRate();

	_tabLength = sm._tabLength;
	delete[] _data;
	delete[] _origData;
	_data = new double[_tabLength];
	_origData = new double[_tabLength];
	_useOLA = sm._useOLA;

	_fft.reset(sm._fft->clone());
	_watermark.reset(sm._watermark->clone());

	_fft->updateSize(sm._fft->size());
	onFFTSizeUpdate();
	std::copy_n(sm._data, _tabLength, _data);
	std::copy_n(sm._origData, _tabLength, _origData);

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


void WatermarkManager::copyInput(const unsigned int pos)
{
	if(_useOLA)
		copyInputOLA(pos);
	else
		copyInputSimple(pos);
}

void WatermarkManager::copyOutput(const unsigned int pos)
{
	if(_useOLA)
		copyOutputOLA(pos);
	else
		copyOutputSimple(pos);
}


WatermarkManager::~WatermarkManager()
{
	_watermark.reset();

	delete[] _data;
	delete[] _origData;
}

void WatermarkManager::initDataArray()
{
	std::copy_n(_origData, _tabLength, _data);
}

double *WatermarkManager::getData() const
{
	return _data;
}

void WatermarkManager::onDataUpdate()
{
	_watermark->onDataUpdate();
}

double *WatermarkManager::getNoisyData()
{
	return _origData;
}

unsigned int WatermarkManager::getLength() const
{
	return _tabLength;
}

unsigned int WatermarkManager::readFile(const char *str)
{
	std::ifstream ifile(str, std::ios_base::ate | std::ios_base::binary);
	_tabLength = ((unsigned int) ifile.tellg()) / (unsigned int) (sizeof(short) / sizeof(char));
	ifile.clear();
	ifile.seekg(0, std::ios_base::beg);

	delete[] _origData;
	delete[] _data;
	_origData = new double[_tabLength];
	_data = new double[_tabLength];

	unsigned int pos = 0;
	short sample;

	while (ifile.read((char *)&sample, sizeof(short)))
	{
		_origData[pos++] = MathUtil::ShortToDouble(sample);
	}

	ifile.close();
	_dataSource = DataSource::File;
	return _tabLength;
}

unsigned int WatermarkManager::readBuffer(const short *buffer, const unsigned int length)
{
	_tabLength = length;

	delete[] _origData;
	delete[] _data;
	_origData = new double[_tabLength];
	_data = new double[_tabLength];

	// Julius accepts only big-endian raw files but it seems internal buffers
	// are little-endian so no need to convert.
	// std::transform(buffer, buffer + tab_length, buffer,
	//                [] (short val) {return (val << 8) | ((val >> 8) & 0xFF)});

	std::transform(buffer, buffer + _tabLength, _origData, MathUtil::ShortToDouble);
	initDataArray();

	_dataSource = DataSource::Buffer;
	return _tabLength;
}

void WatermarkManager::writeBuffer(short * const buffer) const
{
	std::transform(_data, _data + _tabLength, buffer, MathUtil::DoubleToShort);

	// Julius accepts only big-endian raw files but it seems internal buffers
	// are little-endian so no need to convert.
	// std::transform(buffer, buffer + tab_length, buffer,
	//                [] (short val) {return (val << 8) | ((val >> 8) & 0xFF)});
}

void WatermarkManager::copyInputSimple(const unsigned int pos)
{
	// Data copying
	if (_fft->size() <= _tabLength - pos)
	{
		std::copy_n(_data + pos, _fft->size(), _fft->input());
	}
	else
	{
		std::copy_n(_data + pos, _tabLength - pos, _fft->input());
		std::fill_n(_fft->input() + _tabLength - pos, _fft->size() - (_tabLength - pos), 0);
	}
}

void WatermarkManager::copyOutputSimple(const unsigned int pos)
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

void WatermarkManager::copyInputOLA(const unsigned int pos)
{
	// Data copying
	if (_ola_frame_increment <= _tabLength - pos) // last case
	{
		std::copy_n(_data + pos, _ola_frame_increment, _fft->input());
		std::fill_n(_fft->input() + _ola_frame_increment, _ola_frame_increment, 0);

		std::fill_n(_data + pos, _ola_frame_increment, 0);
	}
	else
	{
		std::copy_n(_data + pos, _tabLength - pos, _fft->input());
		std::fill_n(_fft->input() + _tabLength - pos, _ola_frame_increment - (_tabLength - pos), 0);

		std::fill_n(_data + pos, _tabLength - pos, 0);
	}
}

void WatermarkManager::copyOutputOLA(const unsigned int pos)
{
	// Lock here
	//ola_mutex.lock();
	for (auto j = 0U; (j < _fft->size()) && (pos + j < _tabLength); ++j)
	{
		_data[pos + j] += _fft->output()[j] / _fft->size();
	}
	// Unlock here
	//ola_mutex.unlock();
}
bool WatermarkManager::OLAenabled() const
{
	return _useOLA;
}

void WatermarkManager::enableOLA()
{
	_useOLA = true;
}

void WatermarkManager::disableOLA()
{
	_useOLA = false;
}

void WatermarkManager::setOLA(const bool val)
{
	_useOLA = val;
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
