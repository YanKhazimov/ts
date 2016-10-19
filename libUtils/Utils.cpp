#include "Utils.h"

namespace ts
{
	void WindowTimeChecker::RegisterTick ()
	{
		std::chrono::steady_clock::time_point curTick = std::chrono::steady_clock::now ();

		long cycle =
			nullifier * std::chrono::duration_cast<std::chrono::milliseconds> (curTick - prevTick).count ();

		nullifier = 1;

		prevCycle = cycle;

		if (cycle > maxCycle)
		{
			maxCycle = cycle;
		}

		prevTick = curTick;
	}

	sf::String WindowTimeChecker::Report () const
	{
		return Helpers::ToString (prevCycle) + "ms (max=" + Helpers::ToString (maxCycle) + "ms)";
	}

	int WindowTimeChecker::GetId ()
	{ 
		return -1;
	}

	float DriversInput::ResultingTurning () const
	{
		return abs (turnLeft + turnRight) < ZERO_FLOAT ? 0.f : (turnLeft + turnRight);
	}

	float DriversInput::ResultingAcceleration () const
	{
		return abs (driveFwd + driveBwd) < ZERO_FLOAT ? 0.f : (driveFwd + driveBwd);
	}

	float NormalizeAngle (float angle)
	{
		while (angle >= 360.f)
		{
			angle -= 360.f;
		}

		while (angle < -0.f)
		{
			angle += 360.f;
		}

		return angle; // [0; 360)
	}

	float NormalizeAbsAngle (float angle)
	{
		float angleSign = angle < 0.f ? -1.f : 1.f;
		while (angle * angleSign >= 360.f)
		{
			angle -= 360.f * angleSign;
		}

		return angle; // (-360; 360)
	}
}