#pragma once
#include <memory>

#include "copystyle/CopyStyle.h"
#include "../Parameters.h"
#include "../Data.h"

template <typename data_type>
class IOManagerBase
{
	public:
		using size_type = typename Parameters<data_type>::size_type;

		IOManagerBase(const Parameters<data_type>& cfg):
			conf(cfg)
		{

		}

		virtual ~IOManagerBase() = default;


		std::vector<data_type> _baseData = {};
		size_type _pos = 0;

		const Parameters<data_type>& conf;
};
