#pragma once
#include <iostream>
#include <memory>

#include "IOManagerBase.h"
#include "copystyle/InputSimple.h"

template <typename data_type>
/**
 * @brief The InputManagerBase class
 *
 * Classe de base pour les entrées.
 */
class InputManagerBase : public IOManagerBase<data_type>, public InputManagerInterface
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

		virtual Audio_p getNextBuffer() override
		{
			if(pos() == 0) // Premier buffer
			{
				auto d = new CData<data_type>;

				d->_data.resize(channels());
				for(auto& channel : d->_data)
					channel.resize(this->conf.bufferSize);

				buffer.reset(d);
			}

			if(pos() < frames())
			{
				// Remplissage pour chaque canal
				for(auto i = 0U; i < channels(); ++i)
				{
					copyHandler->copy(v()[i].begin(),
									  static_cast<CData<data_type>*>(buffer.get())->_data[i].begin(),
									  pos(),
									  frames());
				}

				pos() += copyHandler->frameIncrement();
				return buffer;
			}

			return Audio_p(nullptr);
		}

		InputCopy_p<data_type> copyHandler = nullptr;

	private:
		Audio_p buffer;
};

