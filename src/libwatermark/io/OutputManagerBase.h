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
			IOManagerBase<data_type>::_baseData.resize(IOManagerBase<data_type>::_baseData.size() + IOManagerBase<data_type>::conf.bufferSize);

			_copy->copy(buffer.begin(),
						IOManagerBase<data_type>::_baseData.begin(),
						IOManagerBase<data_type>::_pos,
						IOManagerBase<data_type>::conf.bufferSize,
						IOManagerBase<data_type>::_baseData.size());

			IOManagerBase<data_type>::_pos += _copy->frameIncrement();
		}


};

typedef std::shared_ptr<IOutputManager> Output_p;
