#pragma once
#include "AtTime.h"


class Every : virtual public AtTime
{
	public:
		Every() = default;
		Every(long unsigned int time, long unsigned int space):
			AtTime(time),
			spaceTime(space)
		{
		}

		virtual bool startCheck() override
		{
			return (count >= startTime) && !(count % (startTime + spaceTime));
		}

		void setSpace(long unsigned int t)
		{
			spaceTime = t;
		}

	protected:
		long unsigned int spaceTime = 0;
};
