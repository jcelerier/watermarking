#pragma once
#include "AtTime.h"

class For : virtual public AtTime
{
	public:
		For() = default;
		For(long unsigned int time, long unsigned int duration):
			AtTime(time),
			durationTime(duration)
		{
		}

		virtual bool stopCheck() override
		{
			return count == (startTime + durationTime);
		}

		void setDuration(long unsigned int t)
		{
			durationTime = t;
		}

	protected:
		long unsigned int durationTime = 0;
};
