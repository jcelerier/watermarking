#pragma once
#include <algorithm>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class BufferInput : public InputManagerBase
{
	public:
		void readBuffer(short * buffer, const size_type length)
		{
			_baseData.resize(length);

			//Faire une conversion de plusieurs types d'entiers en utilisant template & spécialisations ?
			// TODO : typeid ? template ?

			std::transform(buffer, buffer + length, _baseData.begin(), MathUtil::ShortToDouble);
		}
};
