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

	//ts::CStraightLaneSection section ({ 800.f, 300.f }, { 700.f, 200.f });
	ts::CStraightLaneSection section1 ({ 700.f, 700.f }, { 600.f, 600.f });
	ts::CStraightLaneSection section2 ({ 900.f, 300.f }, { 900.f, 200.f });
	ts::CStraightLaneSection section3 ({ 500.f, 500.f }, { 400.f, 500.f });
	ts::CStraightLaneSection section4 ({ 200.f, 200.f }, { 300.f, 350.f });
	ts::CArcLaneSection arc ({ 1000.f, 500.f }, 180.f, -270.f, 100.f);

	/*
		ts::CCar car1 { carTexture1, sf::Color::Green, 100.f, 100.f,
			std::shared_ptr<ts::CCarPhysics> (new ts::CCarLinearPhysics (window)) };
		ts::CControlledCar car2 { carTexture1, sf::Color::Blue, ts::carW / 2, ts::windowH / 2,
			std::shared_ptr<ts::CCarPhysics> (new ts::CCarRealisticPhysics (window)), ts::CCarJoystickControls (0) };
		ts::CControlledCar car3 { carTexture1, sf::Color::Red, ts::carW / 2, ts::windowH / 2 + ts::carH,
			std::shared_ptr<ts::CCarPhysics> (new ts::CCarRealisticPhysics (window)),
			ts::CCarKeyboardControls (sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D) };
	*/
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

	monitor << car1 << car2 << car3;

	std::shared_ptr<ts::CArcLaneSection> cArc;

	while (window.isOpen ())
	{
		windowCycles->RegisterTick ();

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
			arc.Stretch ();
			if (cArc)
			{
				cArc->Stretch ();
			}
		}
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Subtract))
		{
			arc.Shrink ();
			if (cArc)
			{
				cArc->Shrink ();
			}
		}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num2))
		{
			cArc = std::make_shared<ts::CArcLaneSection> (ts::CArcLaneSection ({ 400.f, 400.f }, -90.f, 180.f, 100.f));
		}

		//section.Draw (window, true);
		section1.Draw (window, true);
		section2.Draw (window, true);
		section3.Draw (window, true);
		section4.Draw (window, true);
		arc.Draw (window, true);

		if (cArc)
		{			
			cArc->Draw (window, true);
		}

		/*		
		sf::String buttonStr = "my: ";
		buttonStr += "(" + ts::Helpers::ToString (car3.x ()) + "," + ts::Helpers::ToString (car3.y ()) + ")\n";
		buttonStr += "left   =" + ts::Helpers::ToString (car3.left ()) + "\n";
		buttonStr += "right  =" + ts::Helpers::ToString (car3.right ()) + "\n";
		buttonStr += "top    =" + ts::Helpers::ToString (car3.top ()) + "\n";
		buttonStr += "bottom =" + ts::Helpers::ToString (car3.bottom ()) + "\n";
		sf::Text report (buttonStr, monitor.font, 40);
		report.setPosition ({ 0.f, 400.f });
		window.draw (report);

		car1->Update (window);
		car1->Draw (window);

		car2->Update (window);
		car2->Draw (window);

		car3->Update (window);
		car3->Draw (window);*/

		monitor.Update ();

		window.display ();

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Escape) && window.hasFocus ())
		{
			window.close ();
		}
	}

	return 0;
}