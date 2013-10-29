#pragma once
#include "IOManagerBase.h"
#include "copystyle/OutputSimple.h"
#include <iostream>

class OutputManagerBase :  public IOManagerBase
{
	public:
		OutputManagerBase(const Parameters& cfg):
			IOManagerBase(cfg),
			_copy(new OutputSimple(cfg))
		{

		}

		OutputManagerBase(Output* copy, const Parameters& cfg):
			IOManagerBase(cfg),
			_copy(copy)
		{

		}

		virtual ~OutputManagerBase() = default;

		// Copie de buffer vers notre buffer interne
		virtual void writeNextBuffer(IData* buffer)
		{
			CData<data_type>* b = dynamic_cast<CData<data_type>*>(buffer);
			_baseData.resize(_baseData.size() + b->_data.size());

			_copy->copy(b->_data.begin(), _baseData.begin(), _pos, conf.bufferSize, _baseData.size());

			_pos += _copy->frameIncrement();
		}

	protected:
		std::shared_ptr<Output> _copy = nullptr;
};
