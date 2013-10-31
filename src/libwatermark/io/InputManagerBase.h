#pragma once
#include <iostream>

#include "IOManagerBase.h"
#include "copystyle/InputSimple.h"

class IInputManager
{
	public:
		virtual IData* getNextBuffer() = 0;
		virtual ~IInputManager() = default;
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

		InputManagerBase(const InputManagerBase<data_type>& orig):
			IOManagerBase<data_type>(orig.conf),
			_copy(*orig._copy.get())
		{
		}

		InputManagerBase(Input<data_type>* copy, const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			_copy(copy)
		{
		}

		virtual ~InputManagerBase() = default;

		// Renvoie nullptr quand plus rien
		virtual IData* getNextBuffer() override
		{
			if(this->pos() < this->data().size())
			{
				auto buffer = new CData<data_type>;

				buffer->_data.resize(this->conf.bufferSize);
				_copy->copy(this->data().begin(),
							buffer->_data.begin(),
							this->pos(),
							this->data().size(),
							this->conf.bufferSize);

				this->pos() += _copy->frameIncrement();
				return buffer;
			}

			return nullptr;
		}
};

typedef std::shared_ptr<IInputManager> Input_p;
