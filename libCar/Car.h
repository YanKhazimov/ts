#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "libUtils/Utils.h"
#include "UserControls.h"
#include "CarPhysics.h"

namespace ts
{
	class CCar : public IMonitorableObject
	{
	protected:
		sf::Sprite sprite;
		std::shared_ptr<CCarPhysics> physics;
		DriversInput driversInput { 0.f, 0.f, 0.f, 0.f };

		virtual void FixDriversInput ();

	public:

		CCar (const sf::Texture& texture, const sf::Color& color,
			float posX, float posY, std::shared_ptr<CCarPhysics> carPhysics);

		sf::String Report () const;

		void Update (const sf::Window& window);
		void Draw (sf::RenderWindow& window);

		float x () const;
		float y () const;

		float left () const;
		float right () const;
		float top () const;
		float bottom () const;
	};

	typedef std::shared_ptr<CCar> PCar;

	class CControlledCar : public CCar
	{
		const CCarControls& userControls;

		void FixDriversInput ();

	public:		
		CControlledCar (const sf::Texture& texture, const sf::Color& color, float posX, float posY,
			std::shared_ptr<CCarPhysics> carPhysics, const CCarControls& controls);
	};

	typedef std::shared_ptr<CControlledCar> PControlledCar;
}