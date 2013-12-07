#pragma once
#include "AtTime.h"

/**
 * @brief The For class
 *
 * Déclenche l'algorithme à partir d'un certain temps,
 * et l'arrête au bout de duration.
 */
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
