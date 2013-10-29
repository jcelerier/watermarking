#include "fftmanager.h"
#include <algorithm>

FFTManager::FFTManager(const Parameters & config):
	conf(config)
{
}

FFTManager::~FFTManager()
{

}


FFTManager::size_type FFTManager::spectrumSize() const
{
	return conf.bufferSize / 2 + 1;
}

void FFTManager::updateSize()
{

}
