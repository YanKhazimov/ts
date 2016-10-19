#define _USE_MATH_DEFINES

#pragma once
#include <iostream>
#include <math.h>
#include <Thor/Shapes.hpp>
#include <SFML/Graphics.hpp>
#include "libUtils/Constants.h"
#include "libUtils/Utils.h"

namespace ts
{
	class CLaneSection
	{
	protected:
		float dotR { 3.f };
		sf::CircleShape dot { dotR, 10 };

		const float enlargeDelta { 3.0 };

		CLaneSection ()
		{
			dot.setPosition ({ 100.f, 100.f });
			dot.setFillColor (sf::Color::White);
			dot.setOutlineColor (sf::Color::Black);
			dot.setOutlineThickness (1.f);
			dot.setOrigin (3.f, 3.f);
		}

	public:
		virtual void Stretch () = 0;
		virtual void Shrink () = 0;
	};

	class CStraightLaneSection : public CLaneSection, public sf::RectangleShape
	{
		sf::Vector2f startPoint, endPoint;

		float mWidth { carH };
		
		static float GetNormal (const sf::Vector2f& end0, const sf::Vector2f& end1);

		void UpdateEndPoint ();

	public:

		CStraightLaneSection (const sf::Vector2f& end0, const sf::Vector2f& end1);
		
		void Draw (sf::RenderWindow& window, bool dbgPts = false);

		void Stretch ();
		void Shrink ();
	};

	class CArcLaneSection : public CLaneSection, public thor::ConcaveShape
	{
	private:
		sf::Vector2f startPoint, endPoint;

		float mWidth { carH };
		float mRadius;
		float mStartNormal;
		float mEndNormal;

		bool mNegativeAngle;

		void SetConcavePoints (const unsigned& pointCount);

		bool EnsureIntegrity ();
		unsigned CalculatePointCount ();

	public:

		CArcLaneSection (const sf::Vector2f& center, const float& radius, const sf::Vector2f& normals);
		CArcLaneSection (const sf::Vector2f& end0, const float& normal, const float& angle, const float& radius);

		void Draw (sf::RenderWindow& window, bool dbgPts = false);

		void Stretch ();
		void Shrink ();
	};
}