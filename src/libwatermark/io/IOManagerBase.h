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
		using size_type = typename Parameters<data_type>::size_type;
		std::vector<std::vector<data_type>> _baseData = {};
		size_type _pos = 0;

	protected:
		const Parameters<data_type>& conf;

	public:
		friend class FFTInputProxy<data_type>;
		friend class FFTOutputProxy<data_type>;

		IOManagerBase(const Parameters<data_type>& cfg):
			conf(cfg)
		{
		}

		virtual ~IOManagerBase() = default;

		virtual std::vector<std::vector<data_type> >& v()
		{
			return _baseData;
		}

		size_type channels()
		{
			return v().size();
		}

		size_type frames()
		{
			return v()[0].size();
		}

		size_type& pos()
		{
			return _pos;
		}
};
