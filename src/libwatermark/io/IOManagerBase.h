#pragma once
#include "copystyle/CopyStyle.h"
#include "../Parameters.h"
#include "../Data.h"
#include <memory>

class CopyStyle;

class IOManagerBase
{
	public:
		using size_type = Parameters::size_type;
		using data_type = double;

		virtual size_type getBufferSize() final;
		virtual void setBufferSize(size_type bufferSize) final;

	protected:
		std::vector<data_type> _baseData = {};
		size_type _pos = 0;

		size_type _bufferSize = 512;
};
