#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"

class FFTOutputProxy : public FFTProxy, public OutputManagerBase
{
	public:
		virtual void writeNextBuffer(IData* buffer)
		{

		}
};


