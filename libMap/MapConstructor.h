#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "LaneSection.h"

namespace ts
{
	class CMapConstructor
	{
		std::vector<std::shared_ptr<ts::CLaneSection>> details;
		sf::Vector2f constructionPivotPoint { 400.f, 400.f };
		float constructionPivotNormal { 0.f };

		std::shared_ptr<ts::CLaneSection> mCurrentDetail;
		ESectionType mCurrentType;

		void ShowNextSection ();

	public:
		CMapConstructor (const ESectionType& currentType, const PLaneSection& currentDetail);

		static bool IsConstructionEvent (const sf::Event& event);
		void ProcessConstructionEvent (const sf::Event& event);

		void DrawMap (sf::RenderWindow& window);
	};
}

