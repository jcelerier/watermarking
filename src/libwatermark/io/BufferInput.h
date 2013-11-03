#pragma once
#include <algorithm>

#include "InputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class BufferInput : public InputManagerBase<data_type>
{
	public:
		BufferInput(Parameters<data_type>& cfg):
			InputManagerBase<data_type>(cfg)
		{
		}

		template<typename external_type>
		void readBuffer(external_type * buffer, const typename Parameters<data_type>::size_type length)
		{
			this->data().resize(length);

			if(typeid(data_type) == typeid(external_type))
			{
				std::copy(buffer, buffer + length, this->data().begin());
			}
			else
			{
				std::transform(buffer, buffer + length, this->data().begin(), MathUtil::ToDouble<external_type>);
			}
		}
};
