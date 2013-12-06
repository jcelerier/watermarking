#pragma once
#include "TimeAdapter.h"

class AtTime : public TimeAdapter
{
	public:
		AtTime() = default;
		AtTime(long unsigned int time):
			startTime(time)
		{
			running = false;
		}

		virtual bool startCheck() override
		{
			return count == startTime;
		}

		virtual bool stopCheck() override
		{
			return false;
		}

		void setStartTime(long unsigned int t)
		{
			startTime = t;
		}

	protected:
		long unsigned int startTime = 0;
};
