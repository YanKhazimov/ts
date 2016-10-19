#pragma once

#include <sstream>
#include <iostream>
#include <chrono>
#include <SFML/System.hpp>
#include "Monitor.h"
#include "Constants.h"

namespace ts
{
	struct Helpers
	{
		template<typename T>
		static sf::String ToString (const T& arg)
		{
			std::stringstream sstream;
			sstream << arg;
			return sstream.str ();
		}
	};

	class WindowTimeChecker : public IMonitorableObject
	{
		long maxCycle { LONG_MIN };
		long nullifier { 0 }; // for the 1st tick
		std::chrono::steady_clock::time_point prevTick;
		long prevCycle;

	public:
		int GetId ();		
		sf::String Report () const;
		void RegisterTick ();
	};

	struct DriversInput
	{
		float driveFwd; // 0.f .. 1.f
		float driveBwd; // -1.f .. 0.f
		float turnLeft; // -1.f .. 0.f
		float turnRight; // 0.f .. 1.f

		float ResultingTurning () const;
		float ResultingAcceleration () const;
	};

	float NormalizeAngle (float angle);
	float NormalizeAbsAngle (float angle);
}