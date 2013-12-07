#pragma once
#include "Every.h"
#include "For.h"

/**
 * @brief The Every_For class
 *
 * Déclenche à partir du temps time, pour une durée duration, tous les space.
 *
 * Exemple :
 * time = 4, space = 3, duration = 2.
 * 1 2 3 4 5 6 7 8 9 . . . . . . .
 * |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
 *        x x       x x       x x
 */
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
			const long long int start = count - startTime;
			if(!(start % (spaceTime + durationTime))) std::cerr << "debut: " << count << std::endl;

			return (start >= 0) &&
					 !(start % (spaceTime + durationTime));
		}

		virtual bool stopCheck() override
		{
			const long long int stop = count - startTime - durationTime;
			if(!(stop % (spaceTime + durationTime))) std::cerr << "fin: " << count << std::endl;

			return (stop >= 0) &&
					 !(stop % (spaceTime + durationTime));
		}
};
