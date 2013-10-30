#pragma once
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
