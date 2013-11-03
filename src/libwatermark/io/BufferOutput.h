#pragma once
#include <algorithm>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
class BufferOutput : public OutputManagerBase<data_type>
{
	public:
		BufferOutput(const Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(cfg)
		{
		}

		template<typename external_type>
		void writeOutBuffer(external_type * address)
		{
			if(typeid(data_type) == typeid(external_type))
			{
				std::copy(this->v().begin(),
						  this->v().end(),
						  address);
			}
			else
			{
				std::transform(this->v().begin(),
							   this->v().end(),
							   address,
							   MathUtil::FromDouble<external_type>);
			}
		}
};
