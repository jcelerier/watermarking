#pragma once
#include <functional>
#include <vector>
class TimeAdapter
{
	public:
		virtual ~TimeAdapter() = default;
		virtual bool check() = 0;

		void increment()
		{
			++count;
		}

		void callResetHandlers()
		{
			for(auto& fun : handlers)
			{
				fun();
			}
		}

		void addHandler(std::function<void(void)> f)
		{
			handlers.push_back(f);
		}

	protected:
		long unsigned int count = 0;

	private:
		std::vector<std::function<void(void)>> handlers;
};

using TimeAdapter_p = std::shared_ptr<TimeAdapter>;
