#pragma once
#include "AtTime.h"


/**
 * @brief The Every class
 *
 * Déclenche un algorithme tous les tant de buffers, à partir d'un temps donné.
 *
 * Exemple : à partir de 20 buffers, déclencher tous les 30 buffers.
 */
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
