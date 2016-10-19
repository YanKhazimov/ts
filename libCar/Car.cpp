#include <iostream>
#include "Car.h"

namespace ts
{
	CCar::CCar (const sf::Texture& texture, const sf::Color& color,
		float posX, float posY, std::shared_ptr<CCarPhysics> carPhysics)
		: physics (carPhysics)
	{
		sprite.setTexture (texture);
		sprite.setPosition (posX, posY);
		sprite.setOrigin (carW / 2, carH / 2);
		sprite.setColor (color);

		id = posX + posY;
	}

	CControlledCar::CControlledCar (const sf::Texture& texture, const sf::Color& color, float posX, float posY,
		std::shared_ptr<CCarPhysics> carPhysics, const CCarControls& controls)
		: CCar (texture, color, posX, posY, carPhysics), userControls (controls)
	{		
	}

	sf::String CCar::Report () const
	{
		return "id = " + Helpers::ToString (id)
			+ "; v = " + Helpers::ToString (physics->GetVelocity ())
			+ "; a = " + Helpers::ToString (physics->GetAcceleration ());
	}

	void CCar::FixDriversInput ()
	{
		driversInput = { 0.5f, 0.f,
			0.1f, 0.f };
	}

	void CControlledCar::FixDriversInput ()
	{
		//std::cout << ((CCarKeyboardControls*)&userControls)->driveFwdKey << std::endl;

		userControls.Read (driversInput);		
	}
	
	void CCar::Update (const sf::Window& window)
	{
		// Read driver's instructions
		FixDriversInput ();

		// Calculate shift for angle and position
		auto movementDelta = physics->CalculateMovement (driversInput, sprite);

		// Apply the changes
		sprite.rotate (movementDelta.z);
		sprite.move ({ movementDelta.x, movementDelta.y });
	}

	void CCar::Draw (sf::RenderWindow& window)
	{
		window.draw (sprite);
	}

	float CCar::x () const { return sprite.getPosition ().x; }
	float CCar::y () const { return sprite.getPosition ().y; }

	float CCar::left () const
	{
		return sprite.getGlobalBounds ().left;
		//return x () - getXproj () / 2;
	}
	float CCar::right () const
	{
		const sf::FloatRect& bounds = sprite.getGlobalBounds ();
		return bounds.left + bounds.width;
		//return x () + getXproj () / 2;
	}
	float CCar::top () const
	{		
		return sprite.getGlobalBounds ().top;
		//return y () - getYproj () / 2;
	}
	float CCar::bottom () const
	{
		const sf::FloatRect& bounds = sprite.getGlobalBounds ();
		return bounds.top + bounds.height;
		//return y () + physics->getYproj () / 2;
	}
}