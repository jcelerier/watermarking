#pragma once
#include <algorithm>

#include "OutputManagerBase.h"
#include "../mathutils/math_util.h"

template <typename data_type>
/**
 * @brief The BufferOutput class
 *
 * Ecrit un gros buffer (genre 2048 samples)
 */
class BufferOutput : public OutputManagerBase<data_type>
{
		using IOManagerBase<data_type>::v;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;
	public:
		BufferOutput(Parameters<data_type>& cfg):
			OutputManagerBase<data_type>(cfg)
		{
		}

		template<typename external_type>
		void writeOutBuffer(external_type * address)
		{
			// 1. Réentrelacement
			auto tmp = MathUtil::interleave(v());

			// 2. Copie
			if(typeid(data_type) == typeid(external_type))
				std::copy(tmp.begin(),
						  tmp.end(),
						  address);
			else
				std::transform(tmp.begin(),
							   tmp.end(),
							   address,
							   MathUtil::FromDouble<external_type>);
		}
};
