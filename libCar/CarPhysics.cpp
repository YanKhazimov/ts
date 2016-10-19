#include "CarPhysics.h"

namespace ts
{
	float CCarLinearPhysics::CalculateRotationShift (const float& turning, const float& resultingMovement)
	{
		return CCarPhysics::CalculateMaxRotationShift (turning, resultingMovement);
	}

	sf::Vector3f CCarLinearPhysics::CalculateMovement (const DriversInput& input,
		const sf::Sprite& location)
	{
		if (input.ResultingAcceleration () == 0.f)
		{
			return sf::Vector3f (0.f, 0.f, 0.f);
		}

		sf::Vector3f movement;

		movement.z = CalculateRotationShift (input.ResultingTurning (), input.ResultingAcceleration ());

		double PI_angle = (location.getRotation () + movement.z) / 180 * M_PI;

		movement.x = linearVelocity * cos (PI_angle);
		movement.y = linearVelocity * sin (PI_angle);

		if (input.driveBwd)
		{
			// backwards movement: invert the offset
			movement.x = -movement.x;
			movement.y = -movement.y;
		}

		// bounds collision check
		CheckMapBoundHit (movement, location.getGlobalBounds ());

		return movement;
	}

	float CCarLinearPhysics::GetVelocity () const
	{
		return linearVelocity;
	}

	float CCarLinearPhysics::GetAcceleration () const
	{
		return 0.f;
	}

	float CCarRealisticPhysics::CalculateRotationShift (const float& turning, const float& resultingMovement)
	{
		float angularVelocity = CCarPhysics::CalculateMaxRotationShift (turning, resultingMovement);

		// correcting turn angle depending on velocity
		float absVelocity = abs (velocity);
		if (absVelocity > fastestTurningVelocity)
		{
			angularVelocity /= absVelocity / fastestTurningVelocity;
		}
		else if (absVelocity < fastestTurningVelocity)
		{
			angularVelocity /= fastestTurningVelocity / absVelocity;
		}

		return angularVelocity;
	}

	sf::Vector3f CCarRealisticPhysics::CalculateMovement (const DriversInput& input,
		const sf::Sprite& location)
	{
		sf::Vector3f movement;

		acceleration = power * input.ResultingAcceleration ();

		// should we consider friction here?

		velocity += acceleration;

		movement.z = CalculateRotationShift (input.ResultingTurning (), velocity);

		double PI_angle = (location.getRotation () + movement.z) / 180 * M_PI;

		movement.x = velocity * cos (PI_angle);
		movement.y = velocity * sin (PI_angle);

		// bounds collision check
		if (CheckMapBoundHit (movement, location.getGlobalBounds()))
		{
			velocity = 0.f;
			acceleration = 0.f;
		}

		return movement;
	}

	float CCarRealisticPhysics::GetVelocity () const
	{
		return velocity;
	}

	float CCarRealisticPhysics::GetAcceleration () const
	{
		return acceleration;
	}

	float CCarPhysics::CalculateMaxRotationShift (const float& turning, const float& resultingMovement)
	{
		if (turning == 0.f)
		{
			return 0.f;
		}

		float angularVelocity = 0.f;

		if (resultingMovement > ZERO_FLOAT)
		{
			angularVelocity += maxAngularVelocity * turning;
		}
		else if (resultingMovement < -ZERO_FLOAT)
		{
			angularVelocity += maxAngularVelocity * -turning;
		}

		return angularVelocity;
	}

	float CCarPhysics::getXproj (const sf::Transformable& currentLocation) const
	{
		// relative angle to the 1st quadrant corner in radians
		float b1 = atan (carH / carW) * 180 / M_PI;
		// relative angle to the 2nd quadrant corner in radians
		float b2 = 180 - b1;
		/// 3rd == 1st, 4th == 2nd as absolute proj length is needed

		const float& rotation = currentLocation.getRotation ();
		// determining the needed corner
		float& b = int (rotation) / 90 % 2 == 0 ? b2 : b1;

		// absolute angle to the needed corner
		float total = rotation + b;
		if (total >= 360.f)
			total -= 360.f;

		// hypotenuse
		float hyp = sqrt (carW * carW + carH * carH);

		// projection on the X axis
		float xproj = abs (cos (total / 180 * M_PI)) * hyp;

		return xproj;
	}

	float CCarPhysics::getYproj (const sf::Transformable& currentLocation) const
	{
		// relative angle to the 1st quadrant corner in radians
		float b1 = atan (carH / carW) * 180 / M_PI;
		// relative angle to the 2nd quadrant corner in radians
		float b2 = 180 - b1;
		/// 3rd == 1st, 4th == 2nd as absolute proj length is needed

		const float& rotation = currentLocation.getRotation ();
		// determining the needed corner
		float& b = int (rotation) / 90 % 2 == 0 ? b1 : b2;

		// absolute angle to the needed corner
		float total = rotation + b;
		if (total >= 360.f)
			total -= 360.f;

		// hypotenuse
		float hyp = sqrt (carW * carW + carH * carH);

		// projection on the Y axis
		float yproj = abs (sin (total / 180 * M_PI)) * hyp;

		return yproj;
	}

	float CCarPhysics::CorrectXShift (const float& dx, const sf::FloatRect& location) const
	{
		sf::Vector2f hSpaceReserve { location.left, window.getSize ().x - (location.left + location.width) };

		if (dx > 0 && dx > hSpaceReserve.y)
		{
			return hSpaceReserve.y;
		}
		if (dx < 0 && -dx > hSpaceReserve.x)
		{
			return -hSpaceReserve.x;
		}

		return dx;
	}

	float CCarPhysics::CorrectYShift (const float& dy, const sf::FloatRect& location) const
	{
		sf::Vector2f vSpaceReserve { location.top, window.getSize ().y - (location.top + location.height) };

		if (dy > 0 && dy > vSpaceReserve.y)
		{
			return vSpaceReserve.y;
		}
		if (dy < 0 && -dy > vSpaceReserve.x)
		{
			return -vSpaceReserve.x;
		}

		return dy;
	}

	bool CCarPhysics::CheckMapBoundHit (sf::Vector3f& movement, const sf::FloatRect& location) const
	{
		float correctedDX = CorrectXShift (movement.x, location);
		float correctedDY = CorrectYShift (movement.y, location);

		if (correctedDX != movement.x || correctedDY != movement.y)
		{
			float coeff = fmin (correctedDX / movement.x, correctedDY / movement.y);

			movement.x *= coeff;
			movement.y *= coeff;
			movement.z *= coeff;

			return true;
		}

		return false;
	}
}