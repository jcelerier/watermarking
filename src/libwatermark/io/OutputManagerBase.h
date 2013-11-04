#pragma once
#include <iostream>

#include "IOManagerBase.h"
#include "copystyle/OutputSimple.h"

class IOutputManager
{
	public:
		virtual void writeNextBuffer(IData* buffer) = 0;
};

template <typename data_type>
class OutputManagerBase :  public IOManagerBase<data_type>, public IOutputManager
{
	protected:
		std::shared_ptr<Output<data_type>> copyHandler = nullptr;

	public:
		using IOManagerBase<data_type>::pos;
		using IOManagerBase<data_type>::v;

		OutputManagerBase(const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(new OutputSimple<data_type>(cfg))
		{
		}

		OutputManagerBase(const OutputManagerBase<data_type>& orig):
			IOManagerBase<data_type>(orig.conf),
			copyHandler(*orig.copyHandler.get())
		{
		}

		OutputManagerBase(Output<data_type>* copy, const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(copy)
		{
		}

		virtual ~OutputManagerBase() = default;

		// Copie de buffer vers notre buffer interne
		virtual void writeNextBuffer(IData* abstract_buffer) override
		{
			auto& buffer = dynamic_cast<CData<data_type>*>(abstract_buffer)->_data;
			// On met le bon nombre de canaux
			if(v().size() != buffer.size())
			{
				v().resize(buffer.size());
			}

			for(auto i = 0U; i < v().size(); ++i)
			{
				v()[i].resize(v()[i].size() + this->conf.bufferSize);

				copyHandler->copy(buffer[i].begin(),
							v()[i].begin(),
							pos(),
							v()[i].size());
			}

			pos() += copyHandler->frameIncrement();
		}
};

typedef std::shared_ptr<IOutputManager> Output_p;
