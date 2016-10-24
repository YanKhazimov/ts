#include "MapViewer.h"

namespace ts
{
	CMapViewer::CMapViewer (sf::RenderWindow& renderWindow, const sf::Mouse::Button& button)
		: window (renderWindow), dragButton (button)
	{
	}
	
	void CMapViewer::AdjustViewScale (const int& delta)
	{
		// delta in [-7;7]	
		float zoomFactor = 1.f + 0.1f * delta;

		currentZoomFactor *= zoomFactor;

		// min scaling limit
		if (currentZoomFactor < 0.01f)
			currentZoomFactor = 0.01f;

		sf::View view = window.getView ();

		if (abs (1.f - currentZoomFactor) < 0.05f)
		{
			// when close enough, magnet to the default scale
			currentZoomFactor = 1.f;
			view.setSize (sf::Vector2f { window.getSize () });
		}
		else
		{
			view.zoom (1.f / zoomFactor);
		}

		window.setView (view);

		window.setTitle (Helpers::ToString (int (currentZoomFactor * 100)) + "%");
	}

	bool CMapViewer::IsViewEvent (const sf::Event::EventType& eventType)
	{
		return eventType == sf::Event::MouseWheelScrolled ||
			eventType == sf::Event::MouseButtonPressed ||
			eventType == sf::Event::MouseButtonReleased ||
			eventType == sf::Event::MouseMoved;
	}

	void CMapViewer::ProcessViewEvent (const sf::Event& event)
	{
		switch (event.type)
		{
		case sf::Event::MouseWheelScrolled:
		{
			AdjustViewScale (event.mouseWheelScroll.delta);
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == dragButton)
			{
				dragMousePosition.x = event.mouseButton.x;
				dragMousePosition.y = event.mouseButton.y;
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			if (event.mouseButton.button == dragButton)
			{
				dragMousePosition.x = dragMousePosition.y = -1;
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			if (sf::Mouse::isButtonPressed (dragButton) &&
				dragMousePosition.x + dragMousePosition.y > 0) // position is not reset to (-1,-1)
			{
				sf::View view = window.getView ();
				view.move ((dragMousePosition.x - event.mouseMove.x) / currentZoomFactor,
					(dragMousePosition.y - event.mouseMove.y) / currentZoomFactor);
				window.setView (view);

				dragMousePosition.x = event.mouseMove.x;
				dragMousePosition.y = event.mouseMove.y;
			}
			break;
		}
		default:
			std::cout << "No processing for event " << event.type << std::endl;
		}
	}
}