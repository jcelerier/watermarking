#pragma once
#include <iostream>

#include "IOManagerBase.h"
#include "copystyle/OutputSimple.h"

template <typename data_type>
class OutputManagerBase :  public IOManagerBase<data_type>
{
	protected:
		OutputCopy_p<data_type> copyHandler = nullptr;

	public:
		using IOManagerBase<data_type>::pos;
		using IOManagerBase<data_type>::v;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		OutputManagerBase(const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(new OutputSimple<data_type>(cfg))
		{
		}

		OutputManagerBase(const OutputManagerBase<data_type>& orig) = delete;

		OutputManagerBase(OutputCopy<data_type>* copy, const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(copy)
		{
		}

		virtual ~OutputManagerBase() = default;

		// Copie de buffer vers notre buffer interne
		virtual void writeNextBuffer(Audio_p& abstract_buffer)
		{
			auto& buffer = static_cast<CData<data_type>*>(abstract_buffer.get())->_data;
			// On met le bon nombre de canaux
			if(channels() != buffer.size())
			{
				v().resize(buffer.size());
			}

			for(auto i = 0U; i < channels(); ++i)
			{
				v()[i].resize(frames() + copyHandler->frameIncrement());

				copyHandler->copy(buffer[i].begin(),
								  v()[i].begin(),
								  pos(),
								  frames());
			}

			pos() += copyHandler->frameIncrement();
		}
};

template<typename T>
using Output_p = std::shared_ptr<OutputManagerBase<T>>;
