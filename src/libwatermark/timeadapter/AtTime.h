#pragma once
#include "TimeAdapter.h"

class AtTime : public TimeAdapter
{
	public:
		virtual bool check() override
		{
			return count >= startTime;
		}

		void setTime(long unsigned int t)
		{
			startTime = t;
		}

	private:
		long unsigned int startTime = 0;
};
