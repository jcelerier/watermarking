#pragma once
#include "InputManagerBase.h"
#include "../mathutils/math_util.h"
#include <algorithm>

class BufferInput : public InputManagerBase
{
	public:
		void readBuffer(short * buffer, const size_type length)
		{
			_baseData.resize(length);

			//Faire une conversion de plusieurs types d'entiers en utilisant template & spécialisations ?
			std::transform(buffer, buffer + length, _baseData.begin(), MathUtil::ShortToDouble);
		}
};
