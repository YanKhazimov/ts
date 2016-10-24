#pragma once

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "libCar/Car.h"
#include "libMap/MapViewer.h"
#include "libMap/LaneSection.h"
#include "libMap/MapConstructor.h"
#include "libUtils/Monitor.h"

int main ()
{
	/*std::cout << cosf (1.f) << std::endl;
	std::cout << cosf (1.f + 20 * M_PI) << std::endl;
	std::cout << cosf (1.f - 20 * M_PI) << std::endl;*/

	sf::RenderWindow window (sf::VideoMode (ts::windowW, ts::windowH), sf::String ("100%"));
	window.setFramerateLimit (60);

	ts::CMapViewer viewer (window);

	sf::Texture carTexture1, grassTexture;
	if (!grassTexture.loadFromFile ("grass.jpg") || !carTexture1.loadFromFile ("car.png"))
	{
		std::cout << "Could not load image" << std::endl;
		return -1;
	}
	carTexture1.setSmooth (true);
	sf::Sprite background (grassTexture, sf::IntRect { 0, 0, ts::windowW, ts::windowH });

	ts::CMonitor monitor (window);

	ts::CMapConstructor constructor (ts::SECTION_STRAIGHT, ts::PStraightLaneSection
	(new ts::CStraightLaneSection ({ 500.f, 500.f } , { 400.f, 400.f })));

	ts::PCar car1 (new ts::CCar (carTexture1, sf::Color::Green, 100.f, 100.f,
		std::shared_ptr<ts::CCarPhysics> (new ts::CCarLinearPhysics (window))));
	ts::PControlledCar car2 (new ts::CControlledCar (carTexture1, sf::Color::Blue, ts::carW / 2, ts::windowH / 2,
		std::shared_ptr<ts::CCarPhysics> (new ts::CCarRealisticPhysics (window)), ts::CCarJoystickControls (0)));
	ts::PControlledCar car3 (new ts::CControlledCar (carTexture1, sf::Color::Red, ts::carW / 2, ts::windowH / 2 + ts::carH,
		std::shared_ptr<ts::CCarPhysics> (new ts::CCarRealisticPhysics (window)),
		ts::CCarKeyboardControls (sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D)));

	std::weak_ptr<ts::WindowTimeChecker> wp;

	//{
		std::shared_ptr<ts::WindowTimeChecker> windowCycles (new ts::WindowTimeChecker);
		monitor << windowCycles;
		wp = windowCycles;
	//}

	//monitor << section1 << section2 << car1 << car2 << car3;

	while (window.isOpen ())
	{
		windowCycles->RegisterTick ();

		if (!window.hasFocus ())
		{
			continue;
		}

		sf::Event event;
		while (window.pollEvent (event))
		{
			if (event.type == sf::Event::Closed)
				window.close ();

			if (ts::CMapViewer::IsViewEvent (event.type))
			{
				viewer.ProcessViewEvent (event);
			}

			if (ts::CMapConstructor::IsConstructionEvent (event))
			{
				constructor.ProcessConstructionEvent (event);
			}
		}

		window.clear (sf::Color::White);
		window.draw (background);		

		constructor.DrawMap (window);

				
		car1->Update (window);
		car1->Draw (window);

		car2->Update (window);
		car2->Draw (window);

		car3->Update (window);
		car3->Draw (window);

		monitor.Update ();

		window.display ();

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
		{
			window.close ();
		}
	}

	return 0;
}