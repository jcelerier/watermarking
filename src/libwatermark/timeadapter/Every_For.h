#pragma once
#include "Every.h"
#include "For.h"

class Every_For : public Every, public For
{
	public:
		Every_For() = default;
		Every_For(long unsigned int time, long unsigned int space, long unsigned int duration):
			AtTime(time),
			Every(time, space),
			For(time, duration)
		{
		}

		virtual bool startCheck() override
		{
			int start = count - startTime;
			if(!(start % (spaceTime + durationTime))) std::cerr << "debut: " << count << std::endl;

			return (start >= 0) &&
					 !(start % (spaceTime + durationTime));
		}

		virtual bool stopCheck() override
		{
			int stop = count - startTime - durationTime;
			if(!(stop % (spaceTime + durationTime))) std::cerr << "fin: " << count << std::endl;

			return (stop >= 0) &&
					 !(stop % (spaceTime + durationTime));
		}
};
