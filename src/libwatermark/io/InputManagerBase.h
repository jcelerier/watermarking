#pragma once
#include <iostream>

#include "IOManagerBase.h"
#include "copystyle/InputSimple.h"

class IInputManager
{
	public:
		virtual IData* getNextBuffer() = 0;
};

template <typename data_type>
class InputManagerBase : public IOManagerBase<data_type>, public IInputManager
{
	protected:
		std::shared_ptr<Input<data_type>> _copy = nullptr;

	public:
		InputManagerBase(const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			_copy(new InputSimple<data_type>(cfg))
		{

		}

		InputManagerBase(Input<data_type>* copy, const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			_copy(copy)
		{

		}

		virtual ~InputManagerBase() = default;

		// Renvoie nullptr quand plus rien
		virtual IData* getNextBuffer()
		{
			if(IOManagerBase<data_type>::_pos < IOManagerBase<data_type>::_baseData.size())
			{
				CData<data_type>* b = new CData<data_type>;

				b->_data.resize(IOManagerBase<data_type>::conf.bufferSize);
				_copy->copy(IOManagerBase<data_type>::_baseData.begin(),
							b->_data.begin(),
							IOManagerBase<data_type>::_pos,
							IOManagerBase<data_type>::_baseData.size(),
							IOManagerBase<data_type>::conf.bufferSize);

				IOManagerBase<data_type>::_pos += _copy->frameIncrement();
				return b;
			}

			return nullptr;
		}


};

typedef std::shared_ptr<IInputManager> Input_p;
