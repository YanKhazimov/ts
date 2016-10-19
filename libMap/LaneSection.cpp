#include "LaneSection.h"

namespace ts
{
	CStraightLaneSection::CStraightLaneSection (const sf::Vector2f& end0, const sf::Vector2f& end1)
		: startPoint (end0), endPoint (end1)
	{
		float length = sqrt ((end0.x - end1.x) * (end0.x - end1.x) + (end0.y - end1.y) * (end0.y - end1.y));
		float normal = GetNormal (end0, end1);

		setOrigin (mWidth / 2, length / 2);
		setSize ({ mWidth, length });
		setRotation (normal);
		setFillColor (sf::Color::Color (50, 50, 50));
		setOutlineColor (sf::Color::White);
		setOutlineThickness (2.f);

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
			dot.setPosition (startPoint);
			dot.move ({ dotR * cosf (angle * M_PI / 180.f), dotR * sinf (angle * M_PI / 180.f) });
			dot.setFillColor (sf::Color::Blue);
			window.draw (dot);

			angle = NormalizeAngle (sf::RectangleShape::getRotation () + 90.f);
			dot.setPosition (endPoint);
			dot.move ({ dotR * cosf (angle * M_PI / 180.f), dotR * sinf (angle * M_PI / 180.f) });
			dot.setFillColor (sf::Color::Red);
			window.draw (dot);
		}
	}

	void CStraightLaneSection::Stretch ()
	{
		const sf::Vector2f& oldSize = RectangleShape::getSize ();
		//RectangleShape::setSize ({ oldSize.x, oldSize.y + enlargeDelta });

		//RectangleShape::setRotation (RectangleShape::getRotation () + 1.f);

		float endDx = enlargeDelta * cosf ((sf::RectangleShape::getRotation () + 270.f) * M_PI / 180.f);
		float endDy = enlargeDelta * sinf ((sf::RectangleShape::getRotation () + 90.f) * M_PI / 180.f);

		//endPoint.x += endDx;
		//endPoint.y += endDy;

		const sf::Vector2f& oldPosition = RectangleShape::getPosition ();
		RectangleShape::setPosition ({ oldPosition.x + endDx / 2, oldPosition.y + endDy / 2 });
		
		const sf::Vector2f& oldOrigin = RectangleShape::getOrigin ();
		RectangleShape::setOrigin ({ oldOrigin.x + fabs (endDx) / 2, oldOrigin.y + fabs (endDy) / 2 });

		RectangleShape::move (endDx / 2, endDy / 2);
		//RectangleShape::scale (1.f, (oldSize.y + enlargeDelta) / oldSize.y);

		//UpdateEndPoint ();
	}

	void CStraightLaneSection::Shrink ()
	{

	}

	void CStraightLaneSection::UpdateEndPoint ()
	{
		endPoint.x += enlargeDelta * cosf ((RectangleShape::getRotation () + 180.f) * M_PI / 180.f);
		endPoint.y += enlargeDelta * sinf ((RectangleShape::getRotation () + 270.f) * M_PI / 180.f);
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

		startPoint = end0;
		
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

		endPoint = { center.x + mRadius * cosf ((normal + nAngle) * M_PI / 180.f),
			center.y + mRadius * sinf ((normal + nAngle) * M_PI / 180.f) };

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);

		SetConcavePoints (thor::ConcaveShape::getPointCount ());

		thor::ConcaveShape::setFillColor (sf::Color::Color (50, 50, 50));
		thor::ConcaveShape::setOutlineColor (sf::Color::White);
		thor::ConcaveShape::setOutlineThickness (2.f);
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
		thor::ConcaveShape::setOutlineThickness (2.f);
	}

	void CArcLaneSection::Draw (sf::RenderWindow& window, bool dbgPts)
	{
		if (!EnsureIntegrity ())
			return;

		window.draw (*this);

		if (!dbgPts) return;

		// ends
		float angle = NormalizeAngle (mStartNormal + 90.f);
		dot.setPosition (startPoint);
		dot.move ({ dotR * cosf (angle * M_PI / 180.f), dotR * sinf (angle * M_PI / 180.f) });
		dot.setFillColor (sf::Color::Blue);
		window.draw (dot);

		angle = NormalizeAngle (mEndNormal + 270);
		dot.setPosition (endPoint);
		dot.move ({ dotR * cosf (angle * M_PI / 180.f), dotR * sinf (angle * M_PI / 180.f) });
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
		const sf::Vector2f& center = thor::ConcaveShape::getPosition ();

		if (mNegativeAngle)
		{
			mStartNormal -= 3.f;
			endPoint = { center.x + mRadius * cosf (mStartNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mStartNormal * M_PI / 180.f) };
		}
		else
		{
			mEndNormal += 3.f;
			endPoint = { center.x + mRadius * cosf (mEndNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mEndNormal * M_PI / 180.f) };
		}

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);
		SetConcavePoints (thor::ConcaveShape::getPointCount ());
	}

	void CArcLaneSection::Shrink ()
	{
		const sf::Vector2f& center = thor::ConcaveShape::getPosition ();

		if (mNegativeAngle)
		{
			mStartNormal += 3.f;
			endPoint = { center.x + mRadius * cosf (mStartNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mStartNormal * M_PI / 180.f) };
		}
		else
		{
			mEndNormal -= 3.f;
			endPoint = { center.x + mRadius * cosf (mEndNormal * M_PI / 180.f),
				center.y + mRadius * sinf (mEndNormal * M_PI / 180.f) };
		}

		unsigned pointCount = CalculatePointCount ();
		thor::ConcaveShape::setPointCount (pointCount);
		SetConcavePoints (thor::ConcaveShape::getPointCount ());
	}

}