#pragma once
#include "IOManagerBase.h"
#include <vector>
struct IData
{
		virtual ~IData() = default;
};

template<typename T>
struct CData : public IData
{
	std::vector<T> _data = {};
};

class InputManagerBase : public IOManagerBase
{
	public:
		InputManagerBase() = default;
		virtual ~InputManagerBase() = default;

		// Renvoie nullptr quand plus rien
		virtual IData* getNextBuffer()
		{
			CData<data_type>* b = new CData<data_type>;

			_copy->copy(_origData.begin(), b->_data.begin(), _pos, _dataLength, _bufferSize);

			return b;
		}

	protected:
		std::vector<data_type> _origData = {}; /**< TODO */
		std::shared_ptr<Input> _copy = nullptr;
};
