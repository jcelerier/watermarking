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
		using IOManagerBase<data_type>::pos;

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
			auto channels = this->v().size();
			auto frames = this->v()[0].size();

			if(pos() < frames)
			{
				// Création d'un wrapper qui va contenir les data
				auto buffer = new CData<data_type>;
				buffer->_data.resize(channels);

				// Remplissage pour chaque canal
				for(auto i = 0U; i < channels; ++i)
				{
					buffer->_data[i].resize(this->conf.bufferSize);

					_copy->copy(this->v()[i].begin(),
								buffer->_data[i].begin(),
								pos(),
								frames,
								this->conf.bufferSize);
				}

				this->pos() += _copy->frameIncrement();
				return buffer;
			}

			return nullptr;
		}
};

typedef std::shared_ptr<IInputManager> Input_p;
