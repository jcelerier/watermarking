#pragma once
#include <iostream>

#include "IOManagerBase.h"
#include "copystyle/OutputSimple.h"

template <typename data_type>
/**
 * @brief The OutputManagerBase class
 *
 * Classe de base pour les sorties
 */
class OutputManagerBase :  public IOManagerBase<data_type>, public OutputManagerInterface
{
	private:
		OutputCopy_p<data_type> copyHandler{OutputCopy_p<data_type>(new OutputSimple<data_type>(this->conf))};

	public:
		using IOManagerBase<data_type>::pos;
		using IOManagerBase<data_type>::channels;
		using IOManagerBase<data_type>::frames;

		OutputManagerBase(Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(new OutputSimple<data_type>(cfg))
		{
		}

		OutputManagerBase(const OutputManagerBase<data_type>& orig) = delete;

		OutputManagerBase(OutputCopy<data_type>* copy, Parameters<data_type>& cfg):
			IOManagerBase<data_type>(cfg),
			copyHandler(copy)
		{
		}

		virtual ~OutputManagerBase() {};

		// Copie de buffer vers notre buffer interne
		virtual void writeNextBuffer(Audio_p& data) override
		{
			auto& buffer = getAudio<data_type>(data);
			// On met le bon nombre de canaux
			if(channels() != buffer.size())
			{
				this->v().resize(buffer.size());
				if(copyHandler->frameIncrement() != this->conf.bufferSize)
				for(auto& vec : this->v())
					vec.resize(copyHandler->frameIncrement());
			}

			for(auto i = 0U; i < channels(); ++i)
			{
				this->v()[i].resize(this->v()[i].size() + copyHandler->frameIncrement());

				copyHandler->copy(buffer[i].begin(),
								  this->v()[i].begin(),
								  pos(),
								  frames());
			}

			pos() += copyHandler->frameIncrement();
		}
};

