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
	class CLaneSection : public IMonitorableObject
	{
	protected:
		float mWidth { carH };

		sf::Vector2f mStartPoint, mEndPoint;

		float dotR { 3.f };
		sf::CircleShape dot { dotR, 10 };

		const float enlargeDelta { 3.0 };

		CLaneSection (const sf::Vector2f& end0 = {0.f, 0.f}, const sf::Vector2f& end1 = { 0.f, 0.f })
			: mStartPoint (end0), mEndPoint (end1)
		{
			dot.setPosition ({ 100.f, 100.f });
			dot.setFillColor (sf::Color::White);
			dot.setOutlineColor (sf::Color::Black);
			dot.setOutlineThickness (1.f);
			dot.setOrigin (dotR, dotR);
		}

	public:
		virtual const float& GetStartNormal () = 0;
		virtual const float& GetEndNormal () = 0;
		const sf::Vector2f& GetStartPoint ();
		const sf::Vector2f& GetEndPoint ();

		virtual void Stretch () = 0;
		virtual void Shrink () = 0;
		virtual void Enlarge () = 0;
		virtual void Lessen () = 0;
		virtual void Flip () = 0;

		virtual void Draw (sf::RenderWindow& window, const sf::Color& border, bool dbgPts) = 0;
	};
	typedef std::shared_ptr<CLaneSection> PLaneSection;

	enum ESectionType
	{
		SECTION_STRAIGHT = 0,
		SECTION_ARC,
		SECTION_COUNT
	};

	class CStraightLaneSection : public CLaneSection, public sf::RectangleShape
	{
	private:
		
		static float GetNormal (const sf::Vector2f& end0, const sf::Vector2f& end1);

	public:

		CStraightLaneSection (const sf::Vector2f& end0, const sf::Vector2f& end1);
		CStraightLaneSection (const sf::Vector2f& end0, const float& normal, const float& length);
		
		void Draw (sf::RenderWindow& window, const sf::Color& border = sf::Color::White, bool dbgPts = false);

		void Stretch ();
		void Shrink ();
		void Enlarge ();
		void Lessen ();
		void Flip ();

		const float& GetStartNormal ();
		const float& GetEndNormal ();

		sf::String Report () const;
	};
	typedef std::shared_ptr<CStraightLaneSection> PStraightLaneSection;

	class CArcLaneSection : public CLaneSection, public thor::ConcaveShape
	{
	private:
		float mRadius;
		float mStartNormal;
		float mEndNormal;
		bool mNegativeAngle;

		void SetConcavePoints (const unsigned& pointCount);

		bool EnsureIntegrity ();
		unsigned CalculatePointCount ();

		CArcLaneSection& operator = (const CArcLaneSection& other);

	public:

		CArcLaneSection (const sf::Vector2f& center, const float& radius, const sf::Vector2f& normals);
		CArcLaneSection (const sf::Vector2f& end0, const float& normal, const float& angle, const float& radius);

		void Draw (sf::RenderWindow& window, const sf::Color& border = sf::Color::White, bool dbgPts = false);

		void Stretch ();
		void Shrink ();
		void Enlarge ();
		void Lessen ();
		void Flip ();

		const float& GetStartNormal ();
		const float& GetEndNormal ();

		sf::String Report () const;
	};
	typedef std::shared_ptr<CArcLaneSection> PArcLaneSection;
}