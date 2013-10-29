#pragma once
#include <vector>

struct IData
{
		virtual ~IData() = default;
};

template<typename T>
struct CData : public IData
{
		CData() = default;
		CData(std::vector<T>&& v):
			_data(v)
		{

		}

	std::vector<T> _data = {};
};
