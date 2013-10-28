#pragma once
#include "InputManagerBase.h"
#include "../mathutils/math_util.h"
#include <algorithm>

class BufferInput : public InputManagerBase
{
	public:
		BufferInput();

		/**
		 * @brief Reads a buffer into the internal buffer.
		 *
		 * @param buffer Short buffer to read from.
		 * @param length Length of the buffer.
		 * @return unsigned int Length of the buffer (useless?).
		 */
		void readBuffer(short * buffer, const size_type length)
		{
			_dataLength = length;

			_origData.resize(_dataLength);

			//Faire une conversion de plusieurs types d'entiers en utilisant template & spécialisations ?
			std::transform(buffer, buffer + _dataLength, _origData.begin(), MathUtil::ShortToDouble);
		}

		short* getAddress()
		{
			return _buffer_address;
		}



	private:
		short * _buffer_address = nullptr;
};
