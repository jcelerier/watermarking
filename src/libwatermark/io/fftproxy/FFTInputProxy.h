#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"

class FFTInputProxy : public FFTProxy, public InputManagerBase
{
	public:
		virtual IData* getNextBuffer()
		{

			return nullptr;
		}
};


