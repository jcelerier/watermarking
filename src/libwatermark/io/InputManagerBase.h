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
		InputManagerBase():
			_copy(new InputSimple)
		{

		}

		virtual ~InputManagerBase() = default;

		// Renvoie nullptr quand plus rien
		virtual IData* getNextBuffer()
		{
			if(_pos < _baseData.size())
			{
				CData<data_type>* b = new CData<data_type>;

				_copy->copy(_baseData.begin(), b->_data.begin(), _pos, _baseData.size(), _bufferSize);

				return b;
			}

			_pos += _copy->frameIncrement();
			return nullptr;
		}

	protected:
		std::shared_ptr<Input> _copy = nullptr;
};
