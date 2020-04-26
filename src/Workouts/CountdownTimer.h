/**
*  @file CountdownTimer.h
*
*  Countdown timer class
*
*  Copyright 2020 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

class CountdownTimer
{
public:
	CountdownTimer(int step = 1000)
	    : step(step)
	{
	}
	void start(int value)
	{
		ticks = nextTicks = value;
	}

	bool update(long int delta)
	{
		ticks -= delta;
		if (ticks > 0)
		{
			if (ticks <= nextTicks)
			{
				nextTicks -= step;
				return true;
			}
		}
		else if (ticks < 0)
			ticks = 0;
		return false;
	}

	int getTicks() const { return ticks; }
	bool isElapsed() const { return  ticks <= 0; }

private:
	int const step;
	int ticks { 0 };
	int nextTicks { 0 };
};
