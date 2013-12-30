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
		Output_p outputImpl = nullptr;

	public:
		MCLTOutputProxy(Output_p output,
					  Parameters<data_type>& cfg):
			MCLTProxy<data_type>(cfg),
			OutputManagerBase<data_type>(nullptr, cfg),
			outputImpl(output)
		{
		}

		virtual ~MCLTOutputProxy() = default;
		virtual void writeNextBuffer(Audio_p& data) final override
		{
			auto& buffer = getSpectrum<data_type>(data);
			for(auto& channel : buffer)
				mclt.backward(channel);

			outputImpl->writeNextBuffer(data);
		}

};
