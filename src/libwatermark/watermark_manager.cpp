#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include "watermark_manager.h"
#include "watermark/algorithms.h"
#include "mathutils/math_util.h"
#include "fft/fftwmanager.h"
#include "io/FileInput.h"
//TODO Passer la FFT en proxy.

WatermarkManager::WatermarkManager(const Parameters& parameters):
	_fft(new FFTWManager)
{
	_fft->updateSize(parameters.bufferSize);
	onFFTSizeUpdate();
}

WatermarkManager::WatermarkManager(const WatermarkManager &sm):
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
	_fft.reset(sm._fft->clone());
	_watermark.reset(sm._watermark->clone());

	_fft->updateSize(sm._fft->size());
	onFFTSizeUpdate();
	return *this;
}



void WatermarkManager::execute()
{

	//
//	while(double* buf = _data_in.getNextBuffer())
//	{
//		(*_watermark(buf, data_to_encode));

//		_data_out.setNextBuffer(buf);
//		delete buf;
//	}
}




WatermarkManager::~WatermarkManager()
{
	_watermark.reset();
}


void WatermarkManager::onDataUpdate()
{
	_watermark->onDataUpdate();
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
