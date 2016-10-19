#include "LaneSection.h"

namespace ts
{
	CStraightLaneSection::CStraightLaneSection (const sf::Vector2f& end0, const sf::Vector2f& end1)
		: CLaneSection (end0, end1)
	{
		float length = sqrt ((end0.x - end1.x) * (end0.x - end1.x) + (end0.y - end1.y) * (end0.y - end1.y));
		
		setOrigin (mWidth / 2, length / 2);
		setSize ({ mWidth, length });
		setRotation (GetNormal (end0, end1));
		setFillColor (sf::Color::Color (50, 50, 50));
		setOutlineColor (sf::Color::White);
		setOutlineThickness (1.f);

		setPosition ({ (end0.x + end1.x) / 2, (end0.y + end1.y) / 2 });
	}

	float CStraightLaneSection::GetNormal (const sf::Vector2f& end0, const sf::Vector2f& end1)
	{
		sf::Vector2f zeroNormalFromEnd0 { 1.0f, 0.f };
		sf::Vector2f end0end1 { end1.x - end0.x , end1.y - end0.y };

		float scalarProduct = zeroNormalFromEnd0.x * end0end1.x + zeroNormalFromEnd0.y * end0end1.y;
		float zeroNormalFromEnd0Length = 1.f;
		float end0end1Length = sqrt (end0end1.x * end0end1.x + end0end1.y * end0end1.y);
		float angle = acos (scalarProduct / zeroNormalFromEnd0Length / end0end1Length) * 180.0 / M_PI;

		if (end0.y > end1.y)
		{
			angle = 360.f - angle;
		}

		return (angle >= 270.f) ? (angle - 270.f) : (angle + 90.f);
	}

	void CStraightLaneSection::Draw (sf::RenderWindow& window, bool dbgPts)
	{
		window.draw (*this);

		if (dbgPts)
		{
			float angle = NormalizeAngle (sf::RectangleShape::getRotation() + 270.f);
			dot.setPosition (mStartPoint);
			dot.move ({ dotR * cosf (angle * M_PI / 180.f), dotR * sinf (angle * M_PI / 180.f) });
			dot.setFillColor (sf::Color::Blue);
			window.draw (dot);

			angle = NormalizeAngle (sf::RectangleShape::getRotation () + 90.f);
			dot.setPosition (mEndPoint);
			dot.move ({ dotR * cosf (angle * M_PI / 180.f), dotR * sinf (angle * M_PI / 180.f) });
			dot.setFillColor (sf::Color::Red);
			window.draw (dot);
		}
	}

	void CStraightLaneSection::Stretch ()
	{
		const sf::Vector2f& oldSize = RectangleShape::getSize ();
		RectangleShape::setSize ({ oldSize.x, oldSize.y + enlargeDelta });

		float dx = enlargeDelta * cosf ((sf::RectangleShape::getRotation () + 270.f) * M_PI / 180.f);
		float dy = enlargeDelta * sinf ((sf::RectangleShape::getRotation () + 270.f) * M_PI / 180.f);

		const sf::Vector2f& oldPosition = RectangleShape::getPosition ();
		RectangleShape::setPosition ({ oldPosition.x + dx, oldPosition.y + dy });
		
		mEndPoint.x += dx;
		mEndPoint.y += dy;
	}

	void CStraightLaneSection::Shrink ()
	{
		const sf::Vector2f& oldSize = RectangleShape::getSize ();
		if (oldSize.x >= oldSize.y)
		{
			return;
		}

		float limitedDelta = fmin (enlargeDelta, oldSize.y - oldSize.x);
		RectangleShape::setSize ({ oldSize.x, oldSize.y - limitedDelta });

		float dx = limitedDelta * cosf ((sf::RectangleShape::getRotation () + 90.f) * M_PI / 180.f);
		float dy = limitedDelta * sinf ((sf::RectangleShape::getRotation () + 90.f) * M_PI / 180.f);

		const sf::Vector2f& oldPosition = RectangleShape::getPosition ();
		RectangleShape::setPosition ({ oldPosition.x + dx, oldPosition.y + dy });

		mEndPoint.x += dx;
		mEndPoint.y += dy;
	}

	sf::String CStraightLaneSection::Report () const
	{
		std::stringstream sstream;
		sstream << "(" << getOrigin ().x << ","
			<< getOrigin ().y << ")->("
			<< getPosition ().x << ","
			<< getPosition ().y << ")";
		return sstream.str ();
	}

	void CArcLaneSection::SetConcavePoints (const unsigned& pointCount)
	{
		while (mEndNormal <= mStartNormal)
		{
			mEndNormal += 360.f;
		}

		const sf::Vector2f& center = thor::ConcaveShape::getPosition ();

		float mAngleIteration = (mEndNormal - mStartNormal) / (pointCount / 2 - 1);

		float intR { mRadius - mWidth / 2 };
		float extR { mRadius + mWidth / 2 };

		float x, y;
		for (int p = 0; p < pointCount / 2; ++p) // internal radius
		{
			float PI_angle = (mStartNormal + mAngleIteration * p) * M_PI / 180.f;
			x = center.x + intR * cosf (PI_angle);
			y = center.y + intR * sinf (PI_angle);

			thor::ConcaveShape::setPoint (p, { x, y });
		}
		for (int p = pointCount / 2; p < pointCount; ++p) // external radius
		{
			float PI_angle = (mEndNormal - mAngleIteration * (p - pointCount / 2)) * M_PI / 180.f;
			x = center.x + extR * cosf (PI_angle);
			y = center.y + extR * sinf (PI_angle);

			thor::ConcaveShape::setPoint (p, { x, y });
		}
	}

	bool CArcLaneSection::EnsureIntegrity ()
	{
		return thor::ConcaveShape::getPointCount () < 1000 &&
			thor::ConcaveShape::getPointCount () > 2 &&
			thor::ConcaveShape::getPoint (0) != thor::ConcaveShape::getPoint (1);
	}

	unsigned CArcLaneSection::CalculatePointCount ()
	{
		unsigned pointCount = (6 + mRadius * (fabs)(mEndNormal - mStartNormal) / 360.f);
		return (pointCount + 1) / 2 * 2; // make even
	}

	CArcLaneSection::CArcLaneSection (const sf::Vector2f& end0, const float& normal, const float& angle, const float& radius)
		: mRadius (fmax (radius, mWidth / 2)), mNegativeAngle (angle < 0.f)
	{
		float nAngle = NormalizeAbsAngle (angle);

		sf::Vector2f center { end0.x - mRadius * cosf (normal * M_PI / 180.f), end0.y - mRadius * sinf (normal * M_PI / 180.f) };
		thor::ConcaveShape::setPosition (center);
		thor::ConcaveShape::setOrigin (center);

		mStartPoint = end0;
		
		if (mNegativeAngle)
		{
			mStartNormal = normal + nAngle;
			mEndNormal = normal;
		}
		else
		{
			mStartNormal = normal;
			mEndNormal = normal + nAngle;
		}

		mEndPoint = { center.x + mRadius * cosf ((normal + nAngle) * M_PI / 180.f),
			center.y + mRadius * sinf ((normal + nAngle) * M_PI / 180.f) };

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);

		SetConcavePoints (thor::ConcaveShape::getPointCount ());

		thor::ConcaveShape::setFillColor (sf::Color::Color (50, 50, 50));
		thor::ConcaveShape::setOutlineColor (sf::Color::White);
		thor::ConcaveShape::setOutlineThickness (1.f);
	}

	CArcLaneSection::CArcLaneSection (const sf::Vector2f& center, const float& radius, const sf::Vector2f& normals)
		: mRadius (fmax (radius, mWidth / 2)), mStartNormal (normals.x), mEndNormal (normals.y),
		mNegativeAngle (normals.x > normals.y)
	{
		thor::ConcaveShape::setPosition (center);
		thor::ConcaveShape::setOrigin (center);

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);

		SetConcavePoints (thor::ConcaveShape::getPointCount ());

		thor::ConcaveShape::setFillColor (sf::Color::Color (50, 50, 50));
		thor::ConcaveShape::setOutlineColor (sf::Color::White);
		thor::ConcaveShape::setOutlineThickness (1.f);
	}

	void CArcLaneSection::Draw (sf::RenderWindow& window, bool dbgPts)
	{
		if (!EnsureIntegrity ())
			return;

		window.draw (*this);

		if (!dbgPts) return;

		// ends
		float coeff = mNegativeAngle ? -1.f : 1.f;

		float angle = NormalizeAngle (mStartNormal + 90.f);
		dot.setPosition (mStartPoint);
		dot.move ({ dotR * cosf (angle * M_PI / 180.f), coeff * dotR * sinf (angle * M_PI / 180.f) });
		dot.setFillColor (sf::Color::Blue);
		window.draw (dot);

		angle = NormalizeAngle (mEndNormal + 270.f);
		dot.setPosition (mEndPoint);
		dot.move ({ dotR * cosf (angle * M_PI / 180.f), coeff * dotR * sinf (angle * M_PI / 180.f) });
		dot.setFillColor (sf::Color::Red);
		window.draw (dot);

		// perimeter dots
		/*dot.setPosition (thor::ConcaveShape::getPosition ());
		dot.setFillColor (sf::Color::White);
		window.draw (dot);

		const unsigned& pCount = thor::ConcaveShape::getPointCount ();
		float colorShade = 256.f / (pCount - 1);

		for (int i = 0; i < pCount; ++i)
		{
		dot.setPosition ({ thor::ConcaveShape::getPoint (i).x, thor::ConcaveShape::getPoint (i).y });
		window.draw (dot);
		sf::Color currentColor = dot.getFillColor ();
		currentColor.r -= fmin (colorShade, currentColor.r);
		currentColor.g -= fmin (colorShade, currentColor.g);
		currentColor.b -= fmin (colorShade, currentColor.b);
		dot.setFillColor (currentColor);
		}

		dot.setFillColor (sf::Color::White);*/
	}

	void CArcLaneSection::Stretch ()
	{
		if (enlargeDelta >= NormalizeAngle (mStartNormal - mEndNormal))
		{
			return;
		}

		const sf::Vector2f& center = thor::ConcaveShape::getPosition ();

		if (mNegativeAngle)
		{
			mStartNormal -= enlargeDelta;
			mEndPoint = { center.x + mRadius * cosf (mStartNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mStartNormal * M_PI / 180.f) };
		}
		else
		{
			mEndNormal += enlargeDelta;
			mEndPoint = { center.x + mRadius * cosf (mEndNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mEndNormal * M_PI / 180.f) };
		}

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);
		SetConcavePoints (thor::ConcaveShape::getPointCount ());
	}

	void CArcLaneSection::Shrink ()
	{
		if (enlargeDelta >= NormalizeAngle (mEndNormal - mStartNormal))
		{
			return;
		}

		const sf::Vector2f& center = thor::ConcaveShape::getPosition ();
		
		if (mNegativeAngle)
		{
			mStartNormal += enlargeDelta;
			mEndPoint = { center.x + mRadius * cosf (mStartNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mStartNormal * M_PI / 180.f) };
		}
		else
		{
			mEndNormal -= enlargeDelta;
			mEndPoint = { center.x + mRadius * cosf (mEndNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mEndNormal * M_PI / 180.f) };
		}

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);
		SetConcavePoints (thor::ConcaveShape::getPointCount ());
	}

	void CArcLaneSection::Enlarge ()
	{
		mRadius += enlargeDelta;
		
		sf::Vector2f center;

		if (mNegativeAngle)
		{
			center = { mStartPoint.x - mRadius * cosf (mEndNormal * M_PI / 180.f),
				mStartPoint.y - mRadius * sinf (mEndNormal * M_PI / 180.f) };

			mEndPoint = { center.x + mRadius * cosf (mStartNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mStartNormal * M_PI / 180.f) };
		}
		else
		{
			center = { mStartPoint.x - mRadius * cosf (mStartNormal * M_PI / 180.f),
				mStartPoint.y - mRadius * sinf (mStartNormal * M_PI / 180.f) };

			mEndPoint = { center.x + mRadius * cosf (mEndNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mEndNormal * M_PI / 180.f) };
		}

		thor::ConcaveShape::setPosition (center);
		thor::ConcaveShape::setOrigin (center);

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);
		SetConcavePoints (thor::ConcaveShape::getPointCount ());
	}

	void CArcLaneSection::Lessen ()
	{
		if (mRadius <= mWidth)
		{
			return;
		}

		mRadius = fmax (mWidth, mRadius - enlargeDelta);

		sf::Vector2f center;

		if (mNegativeAngle)
		{
			center = { mStartPoint.x - mRadius * cosf (mEndNormal * M_PI / 180.f),
				mStartPoint.y - mRadius * sinf (mEndNormal * M_PI / 180.f) };

			mEndPoint = { center.x + mRadius * cosf (mStartNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mStartNormal * M_PI / 180.f) };
		}
		else
		{
			center = { mStartPoint.x - mRadius * cosf (mStartNormal * M_PI / 180.f),
				mStartPoint.y - mRadius * sinf (mStartNormal * M_PI / 180.f) };

			mEndPoint = { center.x + mRadius * cosf (mEndNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mEndNormal * M_PI / 180.f) };
		}

		thor::ConcaveShape::setPosition (center);
		thor::ConcaveShape::setOrigin (center);

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);
		SetConcavePoints (thor::ConcaveShape::getPointCount ());
	}

	sf::String CArcLaneSection::Report () const
	{
		return "";
	}

}