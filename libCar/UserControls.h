#pragma once
#include <math.h>
#include <SFML/Window.hpp>
#include "libUtils/Utils.h"

namespace ts
{
	class CCarControls
	{
	public:
		virtual void Read (DriversInput& output) const = 0;
	};

	class CCarKeyboardControls : public CCarControls
	{
	private:
		const sf::Keyboard::Key& driveBwdKey;
		const sf::Keyboard::Key& turnLeftKey;
		const sf::Keyboard::Key& turnRightKey;

	public:
		const sf::Keyboard::Key& driveFwdKey;
		CCarKeyboardControls (const sf::Keyboard::Key& fwd = sf::Keyboard::Up,
			const sf::Keyboard::Key& bwd = sf::Keyboard::Down,
			const sf::Keyboard::Key& left = sf::Keyboard::Left,
			const sf::Keyboard::Key& right = sf::Keyboard::Right)
			: driveFwdKey (fwd), driveBwdKey (bwd), turnLeftKey (left), turnRightKey (right) {}

		void Read (DriversInput& output) const;
	};

	class CCarJoystickControls : public CCarControls
	{
	private:
		const float maxButtonPressure = 100.f;

		const float joystickFwdAxisFault = -1.f;
		const float joystickBwdAxisFault = 1.f;
		const float joystickLeftAxisFault = -33.f;
		const float joystickRightAxisFault = 10.f;

		int joystickId { -1 };
		const sf::Joystick::Axis& driveFwdAxis;
		const sf::Joystick::Axis& driveBwdAxis;
		const sf::Joystick::Axis& turnLeftAxis;
		const sf::Joystick::Axis& turnRightAxis;

	public:
		CCarJoystickControls (const int joystick,
			const sf::Joystick::Axis& fwd = sf::Joystick::Axis::Z,
			const sf::Joystick::Axis& bwd = sf::Joystick::Axis::Z,
			const sf::Joystick::Axis& left = sf::Joystick::Axis::X,
			const sf::Joystick::Axis& right = sf::Joystick::Axis::X);

		void Read (DriversInput& output) const;
	};


	/// XBOX 360 layout:

	/// axes:
	// left stick H = sf::Joystick::X
	// left stick V = sf::Joystick::Y
	// left trigger [0,100], right trigger [-100,0] = sf::Joystick::Z
	// right stick V = sf::Joystick::R
	// right stick H = sf::Joystick::U
	// sf::Joystick::V is absent
	// cross H = sf::Joystick::PovX
	// cross V = sf::Joystick::PovY

	/// buttons:
	// 0 = A
	// 1 = B
	// 2 = X
	// 3 = Y
	// 4 = LB
	// 5 = RB
	// 6 = back
	// 7 = start
	// 8 = left stick
	// 9 = right stick
}