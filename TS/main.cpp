#pragma once

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "libCar/Car.h"
#include "libMap/MapViewer.h"
#include "libMap/LaneSection.h"
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

	std::shared_ptr<ts::CStraightLaneSection> section1
	(new ts::CStraightLaneSection ({ 400.f, 400.f }, { 300.f, 400.f }));
	std::shared_ptr<ts::CStraightLaneSection> section2
	(new ts::CStraightLaneSection ({ 900.f, 300.f }, { 900.f, 200.f }));
	std::shared_ptr<ts::CStraightLaneSection> section3
	(new ts::CStraightLaneSection ({ 700.f, 700.f }, { 600.f, 600.f }));
	std::shared_ptr<ts::CStraightLaneSection> section4
	(new ts::CStraightLaneSection ({ 200.f, 200.f }, { 300.f, 350.f }));
	ts::CArcLaneSection arc ({ 1000.f, 500.f }, 180.f, -135.f, 100.f);

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

	monitor << section1 << section2;// << car1 << car2 << car3;

	std::shared_ptr<ts::CArcLaneSection> cArc;

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
		}

		window.clear (sf::Color::White);
		window.draw (background);

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Add))
		{
			section1->Stretch ();
			section2->Stretch ();
			section3->Stretch ();
			section4->Stretch ();
			arc.Stretch ();
			if (cArc)
			{
				cArc->Stretch ();
			}
		}
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Subtract))
		{
			section1->Shrink ();
			section2->Shrink ();
			section3->Shrink ();
			section4->Shrink ();
			arc.Shrink ();
			if (cArc)
			{
				cArc->Shrink ();
			}
		}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Multiply))
		{
			arc.Enlarge();
			if (cArc)
			{
				cArc->Enlarge ();
			}
		}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Divide))
		{
			arc.Lessen ();
			if (cArc)
			{
				cArc->Lessen ();
			}
		}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Tab))
		{
			arc.Flip ();
			if (cArc)
			{
				cArc->Flip ();
			}
		}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num2))
		{
			cArc = std::make_shared<ts::CArcLaneSection> (ts::CArcLaneSection ({ 400.f, 400.f }, -90.f, 180.f, ts::carH + 10.f));
		}

		section1->Draw (window, true);
		section2->Draw (window, true);
		section3->Draw (window, true);
		section4->Draw (window, true);
		arc.Draw (window, true);

		if (cArc)
		{			
			cArc->Draw (window, true);
		}

		/*		
		car1->Update (window);
		car1->Draw (window);

		car2->Update (window);
		car2->Draw (window);

		car3->Update (window);
		car3->Draw (window);*/

		monitor.Update ();

		window.display ();

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
		{
			window.close ();
		}
	}

	return 0;
}