#pragma once
#include <iostream>
#include <memory>

#include "InputManagerBase.h"

template <typename data_type>
/**
 * @brief The InputManagerBase class
 *
 * Classe de base pour les entrées.
 */
class InputMultiplexer : public InputManagerBase<data_type>
{
	public:
		InputMultiplexer(Input_p i1, Input_p i2, Parameters<data_type>& cfg):
			InputManagerBase<data_type>(cfg),
			input1(i1),
			input2(i2)
		{
		}

		InputMultiplexer(const InputMultiplexer<data_type>& orig) = delete;

		virtual ~InputMultiplexer() = default;

		virtual Audio_p getNextBuffer() override
		{
			auto out = new MultiData;
			out->_data.push_back(input1->getNextBuffer());
			if(!out->_data[0]) return Audio_p(nullptr);
			out->_data.push_back(input2->getNextBuffer());
			if(!out->_data[1]) return Audio_p(nullptr);

			return Audio_p(out);
		}

	private:
		Input_p input1;
		Input_p input2;
};

