#pragma once
#include <vector>

#include "../../Parameters.h"

class ICopyStyle
{
	public:
		virtual ~ICopyStyle() = default;
};
typedef std::shared_ptr<ICopyStyle> Copy_p;

template <typename data_type>
class CopyStyle : public ICopyStyle
{
	public:
		using size_type = typename Parameters<data_type>::size_type;

		CopyStyle(const Parameters<data_type>& cfg):
			conf(cfg)
		{
		}

		virtual ~CopyStyle() = default;
		virtual void copy(typename std::vector<data_type>::const_iterator in,
				  typename std::vector<data_type>::iterator out,
				  size_type pos,
				  size_type big_vector_length) = 0;

		virtual size_type frameIncrement() = 0;

	protected:
		const Parameters<data_type>& conf;
};

