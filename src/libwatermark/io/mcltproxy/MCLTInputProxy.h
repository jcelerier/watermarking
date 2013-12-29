#pragma once

#include "MCLTProxy.h"
#include "../InputManagerBase.h"

template <typename data_type>
class MCLTInputProxy : public MCLTProxy<data_type>, public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

		using MCLTProxy<data_type>::mclt;
		using complex_type = typename Parameters<data_type>::complex_type;

	private:
		Input_p inputImpl = nullptr;

	public:
		MCLTInputProxy(Input_p input,
					  Parameters<data_type>& cfg):
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
			auto& buffer = static_cast<CData<complex_type>*>(tmp.get())->_data;

			for(auto& channel : buffer)
				mclt.forward(channel);

			return tmp;
		}
};
