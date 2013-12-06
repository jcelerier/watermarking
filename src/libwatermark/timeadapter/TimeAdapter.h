#pragma once
#include <functional>
#include <vector>
class TimeAdapter
{
	public:
		virtual ~TimeAdapter() = default;
		virtual bool startCheck() = 0;
		virtual bool stopCheck() = 0;

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

		void perform()
		{
			if(startCheck())
			{
				running = true;
				callResetHandlers();
			}

			if(stopCheck()) running = false;

			increment();
		}

		bool isRunning()
		{
			return running;
		}

	protected:
		long unsigned int count = 0;
		bool running = true;

	private:
		std::vector<std::function<void(void)>> handlers = {};
};

using TimeAdapter_p = std::shared_ptr<TimeAdapter>;
