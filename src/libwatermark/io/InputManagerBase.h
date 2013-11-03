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
			if(this->pos() < this->data()[0].size())
			{
				auto buffer = new CData<data_type>;
				buffer->_data.resize(this->data().size());

				for(auto i = 0U; i < this->data().size(); ++i)
				{
					buffer->_data[i].resize(this->conf.bufferSize);

					_copy->copy(this->data()[i].begin(),
								buffer->_data[i].begin(),
								this->pos(),
								this->data()[i].size(),
								this->conf.bufferSize);
				}
				this->pos() += _copy->frameIncrement();
				return buffer;
			}

			return nullptr;
		}
};

typedef std::shared_ptr<IInputManager> Input_p;
