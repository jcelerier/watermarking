#pragma once

#include "MCLTProxy.h"
#include "../OutputManagerBase.h"

template <typename data_type>
class MCLTOutputProxy : public MCLTProxy<data_type>, public OutputManagerBase<data_type>
{
		using OutputManagerBase<data_type>::channels;
		using OutputManagerBase<data_type>::frames;

		using MCLTProxy<data_type>::mclt;
		using complex_type = typename Parameters<data_type>::complex_type;

	private:
		Output_p<data_type> outputImpl = nullptr;

	public:
		MCLTOutputProxy(Output_p<data_type> output,
					  Parameters<data_type>& cfg):
			MCLTProxy<data_type>(cfg),
			OutputManagerBase<data_type>(nullptr, cfg),
			outputImpl(output)
		{
		}

		virtual ~MCLTOutputProxy()
		{
			std::cerr << "mcltoutputproxy\n";
		}

		virtual void writeNextBuffer(Audio_p& buf) final override
		{
			auto& buffer = static_cast<CData<complex_type>*>(buf.get())->_data;
			for(auto& channel : buffer)
				mclt.backward(channel);

			outputImpl->writeNextBuffer(buf);
		}

};
