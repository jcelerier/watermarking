#pragma once
#include "IOManagerBase.h"
#include <vector>
#include "copystyle/Input.h"
#include "copystyle/InputSimple.h"


// Etapes :
// 1. Chargement depuis l'extérieur dans _baseData
// 2. Découpage en petits buffers par _copy
class InputManagerBase : public IOManagerBase
{
	public:
		InputManagerBase(const Parameters& cfg):
			IOManagerBase(cfg),
			_copy(new InputSimple(cfg))
		{

		}

		InputManagerBase(Input* copy, const Parameters& cfg):
			IOManagerBase(cfg),
			_copy(copy)
		{

		}

		virtual ~InputManagerBase() = default;

		// Renvoie nullptr quand plus rien
		virtual IData* getNextBuffer()
		{
			if(_pos < _baseData.size())
			{
				CData<data_type>* b = new CData<data_type>;
				_copy->copy(_baseData.begin(), b->_data.begin(), _pos, _baseData.size(), conf.bufferSize);

				return b;
			}

			_pos += _copy->frameIncrement();
			return nullptr;
		}

	protected:
		std::shared_ptr<Input> _copy = nullptr;
};
