#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "libUtils/Utils.h"

namespace ts
{
	class CMapViewer
	{
	public:
		CMapViewer (sf::RenderWindow& renderWindow, const sf::Mouse::Button& button = sf::Mouse::Right);
		
		void AdjustViewScale (const int& delta);
		
		static bool IsViewEvent (const sf::Event::EventType& eventType);
		void ProcessViewEvent (const sf::Event& event);

	private:
		sf::RenderWindow& window;

		float currentZoomFactor = 1.f;

		sf::Vector2i dragMousePosition;
		const sf::Mouse::Button& dragButton;
	};

}