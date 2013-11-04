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
		std::shared_ptr<Input<data_type>> copyHandler = nullptr;

	public:
		using IOManagerBase<data_type>::pos;
		using IOManagerBase<data_type>::v;
		InputManagerBase(const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(new InputSimple<data_type>(cfg))
		{
		}

		InputManagerBase(const InputManagerBase<data_type>& orig):
			IOManagerBase<data_type>(orig.conf),
			copyHandler(*orig.copyHandler.get())
		{
		}

		InputManagerBase(Input<data_type>* copy, const Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(copy)
		{
		}

		virtual ~InputManagerBase() = default;

		// Renvoie nullptr quand plus rien
		virtual IData* getNextBuffer() override
		{
			auto channels = v().size();
			auto frames = v()[0].size();

			if(pos() < frames)
			{
				// Création d'un wrapper qui va contenir les data
				auto buffer = new CData<data_type>;
				buffer->_data.resize(channels);

				// Remplissage pour chaque canal
				for(auto i = 0U; i < channels; ++i)
				{
					buffer->_data[i].resize(this->conf.bufferSize);

					copyHandler->copy(v()[i].begin(),
								buffer->_data[i].begin(),
								pos(),
								frames);
				}

				pos() += copyHandler->frameIncrement();
				return buffer;
			}

			return nullptr;
		}
};

typedef std::shared_ptr<IInputManager> Input_p;
