#pragma once
#include <memory>

#include "copystyle/CopyStyle.h"
#include "../Parameters.h"
#include "../Data.h"

template <typename data_type>
class FFTInputProxy;
template <typename data_type>
class FFTOutputProxy;

template <typename data_type>
class IOManagerBase
{
	public:
		friend class FFTInputProxy<data_type>;
		friend class FFTOutputProxy<data_type>;
		using size_type = typename Parameters<data_type>::size_type;

		IOManagerBase(const Parameters<data_type>& cfg):
			conf(cfg)
		{
		}

		virtual ~IOManagerBase() = default;

		std::vector<data_type>& data()
		{
			return _baseData;
		}
		size_type& pos()
		{
			return _pos;
		}

	protected:
		std::vector<data_type> _baseData = {};
		size_type _pos = 0;

		const Parameters<data_type>& conf;
};
