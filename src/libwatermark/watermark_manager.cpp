#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include "watermark_manager.h"
#include "watermark/algorithms.h"
#include "io/FileInput.h"
#include "io/FileOutput.h"
#include "io/fftproxy/FFTInputProxy.h"

WatermarkManager::WatermarkManager(const Parameters& parameters):
	conf(parameters)
{

}

WatermarkManager::WatermarkManager(const WatermarkManager &sm):
	conf(sm.conf)

{
	_watermark.reset(sm._watermark->clone());
}

const WatermarkManager &WatermarkManager::operator=(const WatermarkManager &sm)
{
	_watermark.reset(sm._watermark->clone());

	return *this;
}

void WatermarkManager::execute()
{
	std::cerr << "ici\n";
	while(IData* buf = _input->getNextBuffer())
	{


		(*_watermark)(buf);

		_output->writeNextBuffer(buf);
		delete buf;
	}
}

WatermarkManager::~WatermarkManager()
{
	_watermark.reset();
}

void WatermarkManager::onDataUpdate()
{
	_watermark->onDataUpdate();
}

