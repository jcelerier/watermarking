#pragma once
#include <iostream>
#include <memory>

#include "IOManagerBase.h"
#include "copystyle/InputSimple.h"

template <typename data_type>
class InputManagerBase : public IOManagerBase<data_type>
{
	public:
		using IOManagerBase<data_type>::pos;
		using IOManagerBase<data_type>::v;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		InputManagerBase(Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(new InputSimple<data_type>(cfg))
		{
		}

		InputManagerBase(const InputManagerBase<data_type>& orig) = delete;

		InputManagerBase(InputCopy<data_type>* copy, Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(copy)
		{
		}

		virtual ~InputManagerBase() = default;

		virtual Audio_p getNextBuffer()
		{
			if(pos() < frames())
			{
				auto buffer = new CData<data_type>;
				// Création d'un wrapper qui va contenir les data
				buffer->_data.resize(channels());

				// Remplissage pour chaque canal
				for(auto i = 0U; i < channels(); ++i)
				{
					buffer->_data[i].resize(this->conf.bufferSize);

					copyHandler->copy(v()[i].begin(),
									  buffer->_data[i].begin(),
									  pos(),
									  frames());
				}

				pos() += copyHandler->frameIncrement();
				return Audio_p(buffer);
			}

			return Audio_p(nullptr);
		}

		InputCopy_p<data_type> copyHandler = nullptr;

};

template<typename T>
using Input_p = std::shared_ptr<InputManagerBase<T>>;
