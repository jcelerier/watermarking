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
		std::shared_ptr<Output<data_type>> _copy = nullptr;

	public:
		OutputManagerBase(const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			_copy(new OutputSimple<data_type>(cfg))
		{
		}

		OutputManagerBase(const OutputManagerBase<data_type>& orig):
			IOManagerBase<data_type>(orig.conf),
			_copy(*orig._copy.get())
		{
		}

		OutputManagerBase(Output<data_type>* copy, const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			_copy(copy)
		{
		}

		virtual ~OutputManagerBase() = default;

		// Copie de buffer vers notre buffer interne
		virtual void writeNextBuffer(IData* abstract_buffer) override
		{
			auto& buffer = dynamic_cast<CData<data_type>*>(abstract_buffer)->_data;
			// On met le bon nombre de canaux
			if(this->v().size() != buffer.size())
			{
				this->v().resize(buffer.size());
			}

			for(auto i = 0U; i < this->v().size(); ++i)
			{
				this->v()[i].resize(this->v()[i].size() + this->conf.bufferSize);

				_copy->copy(buffer[i].begin(),
							this->v()[i].begin(),
							this->pos(),
							this->conf.bufferSize,
							this->v()[i].size());
			}
			this->pos() += _copy->frameIncrement();
		}
};

typedef std::shared_ptr<IOutputManager> Output_p;
