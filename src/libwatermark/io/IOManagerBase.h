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
		using data_type = Parameters::data_type;

		IOManagerBase(const Parameters& cfg):
			conf(cfg)
		{

		}

		virtual ~IOManagerBase() = default;

	protected:
		std::vector<data_type> _baseData = {};
		size_type _pos = 0;

		const Parameters& conf;
};
