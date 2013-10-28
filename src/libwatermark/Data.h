#pragma once

struct IData
{
		virtual ~IData() = default;
};

template<typename T>
struct CData : public IData
{
	std::vector<T> _data = {};
};
