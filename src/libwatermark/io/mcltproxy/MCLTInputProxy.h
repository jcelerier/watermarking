#pragma once

#include "MCLTProxy.h"
#include "../InputManagerBase.h"

template <typename data_type>
class MCLTInputProxy : public MCLTProxy<data_type>, public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

		using MCLTProxy<data_type>::mclt;

	private:
		Input_p<data_type> inputImpl = nullptr;

	public:
		MCLTInputProxy(InputManagerBase<data_type>* input,
					  const Parameters<data_type>& cfg):
			MCLTProxy<data_type>(cfg),
			InputManagerBase<data_type>(nullptr, cfg),
			inputImpl(input)
		{
		}

		virtual ~MCLTInputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			// 1. On get le buffer.
			Audio_p tmp = inputImpl->getNextBuffer();
			if(tmp == nullptr) return tmp;
			auto& inbuff = static_cast<CData<complex_type>*>(tmp.get())->_data;
			mclt.forward(inbuff);

			return tmp;
		}
};
