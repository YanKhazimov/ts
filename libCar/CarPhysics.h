#define _USE_MATH_DEFINES

#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>
#include "libUtils/Utils.h"

namespace ts
{
	class CCarPhysics
	{
	protected:
		const sf::Window& window;

		float maxAngularVelocity { 2.f };

		float CalculateMaxRotationShift (const float& turning, const float& resultingMovement); // for any velocity
		virtual float CalculateRotationShift (const float& turning, const float& resultingMovement) = 0;

		float CorrectXShift (const float& dx, const sf::FloatRect& location) const;
		float CorrectYShift (const float& dy, const sf::FloatRect& location) const;
		bool CheckMapBoundHit (sf::Vector3f& movement, const sf::FloatRect& location) const;
		
		CCarPhysics (const sf::Window& physicsWindow)
			: window (physicsWindow) {}

	public:
		float getXproj (const sf::Transformable& currentLocation) const;
		float getYproj (const sf::Transformable& currentLocation) const;

		virtual sf::Vector3f CalculateMovement (const DriversInput& input,
			const sf::Sprite& currentLocation) = 0; // dx, dy, dangle

		virtual float GetVelocity () const = 0;
		virtual float GetAcceleration () const = 0;
	};

	class CCarLinearPhysics : public CCarPhysics
	{
	private:		
		float linearVelocity { 3.f };

		float CalculateRotationShift (const float& turning, const float& resultingMovement);

	public:
		CCarLinearPhysics (const sf::Window& physicsWindow)
			: CCarPhysics (physicsWindow) {}

		sf::Vector3f CalculateMovement (const DriversInput& input,
			const sf::Sprite& currentLocation);

		float GetVelocity () const;
		float GetAcceleration () const;
	};

	class CCarRealisticPhysics : public CCarPhysics
	{
	private:
		float power { 0.1f };
		float fastestTurningVelocity { 2.f };

		float velocity { 0.f };
		float acceleration { 0.f };

		float CalculateRotationShift (const float& turning, const float& resultingMovement);

	public:
		CCarRealisticPhysics (const sf::Window& physicsWindow)
			: CCarPhysics (physicsWindow) {}

		sf::Vector3f CalculateMovement (const DriversInput& input,
			const sf::Sprite& currentLocation);

		float GetVelocity () const;
		float GetAcceleration () const;
	};
}