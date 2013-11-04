#pragma once
#include <vector>
#include <memory>

struct IData
{
		virtual ~IData() = default;
};
using Audio_p = std::unique_ptr<IData>;

template<typename T>
struct CData : public IData
{
	std::vector<std::vector<T>> _data = {};
};


