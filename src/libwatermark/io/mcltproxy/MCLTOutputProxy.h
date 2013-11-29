#pragma once

#include "MCLTProxy.h"
#include "../OutputManagerBase.h"

template <typename data_type>
class MCLTOutputProxy : public MCLTProxy<data_type>, public OutputManagerBase<data_type>
{
		using OutputManagerBase<data_type>::channels;
		using OutputManagerBase<data_type>::frames;

		using MCLTProxy<data_type>::mclt;

	private:
		Output_p<data_type> OutputImpl = nullptr;

	public:
		MCLTOutputProxy(OutputManagerBase<data_type>* Output,
					  const Parameters<data_type>& cfg):
			MCLTProxy<data_type>(cfg),
			OutputManagerBase<data_type>(nullptr, cfg),
			OutputImpl(Output)
		{
		}

		virtual ~MCLTOutputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			// 1. On get le buffer.
			Audio_p tmp = OutputImpl->getNextBuffer();
			if(tmp == nullptr) return tmp;
			auto& inbuff = static_cast<CData<complex_type>*>(tmp.get())->_data;
			mclt.forward(inbuff);

			return tmp;
		}
		virtual void writeNextBuffer(Audio_p& buf) final override
		{
			auto& buffer = static_cast<CData<typename FFTProxy<data_type>::complex_type>*>(buf.get())->_data;
			mclt.backward(buffer);
			outputImpl->writeNextBuffer(buf);
		}

};
