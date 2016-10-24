#include "MapConstructor.h"


namespace ts
{
	void CMapConstructor::ShowNextSection ()
	{
		if (mCurrentType == SECTION_ARC)
		{
			mCurrentDetail = PArcLaneSection (new CArcLaneSection (
				constructionPivotPoint, constructionPivotNormal, 45.f, 100.f));
		}
		else
		{
			mCurrentDetail = PStraightLaneSection (new CStraightLaneSection (
				constructionPivotPoint, constructionPivotNormal, 100.f));
		}
	}

	CMapConstructor::CMapConstructor (const ESectionType& currentType, const PLaneSection& currentDetail)
		: mCurrentType (currentType), mCurrentDetail (currentDetail)
	{
		constructionPivotNormal = mCurrentDetail->GetStartNormal ();
		constructionPivotPoint = mCurrentDetail->GetStartPoint ();
	}

	bool CMapConstructor::IsConstructionEvent (const sf::Event& event)
	{
		return event.type == sf::Event::KeyReleased &&
			(event.key.code == sf::Keyboard::Return ||
				event.key.code == sf::Keyboard::LAlt ||
				event.key.code == sf::Keyboard::Add ||
				event.key.code == sf::Keyboard::Subtract ||
				event.key.code == sf::Keyboard::Multiply ||
				event.key.code == sf::Keyboard::Divide ||
				event.key.code == sf::Keyboard::Tab);
	}

	void CMapConstructor::ProcessConstructionEvent (const sf::Event& event)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Add:
		{
			mCurrentDetail->Stretch ();
			break;
		}
		case sf::Keyboard::Subtract:
		{
			mCurrentDetail->Shrink ();
			break;
		}
		case sf::Keyboard::Multiply:
		{
			mCurrentDetail->Enlarge ();
			break;
		}
		case sf::Keyboard::Divide:
		{
			mCurrentDetail->Lessen ();
			break;
		}
		case sf::Keyboard::Tab:
		{
			mCurrentDetail->Flip ();
			break;
		}
		case sf::Keyboard::LAlt:
		{
			mCurrentType = ESectionType ((mCurrentType + 1) % SECTION_COUNT);

			ShowNextSection ();

			break;
		}
		case sf::Keyboard::Return:
		{
			details.push_back (mCurrentDetail);

			constructionPivotPoint = mCurrentDetail->GetEndPoint ();
			constructionPivotNormal = mCurrentDetail->GetEndNormal ();

			ShowNextSection ();

			break;
		}
		default:
			break;
		}
	}

	void CMapConstructor::DrawMap (sf::RenderWindow& window)
	{
		for (auto& mapDetail : details)
		{
			mapDetail->Draw (window, sf::Color::White, true);
		}

		mCurrentDetail->Draw (window, sf::Color::Yellow, true);
	}

}