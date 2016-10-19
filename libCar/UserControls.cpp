#include "UserControls.h"

namespace ts
{
	void CCarKeyboardControls::Read (DriversInput& output) const
	{
		output =
		{
			(float) sf::Keyboard::isKeyPressed (driveFwdKey),
			-1.f * (float) sf::Keyboard::isKeyPressed (driveBwdKey),
			-1.f * (float) sf::Keyboard::isKeyPressed (turnLeftKey),
			(float) sf::Keyboard::isKeyPressed (turnRightKey)
		};
	}

	CCarJoystickControls::CCarJoystickControls (const int joystick,
		const sf::Joystick::Axis& fwd,
		const sf::Joystick::Axis& bwd,
		const sf::Joystick::Axis& left,
		const sf::Joystick::Axis& right)
		: joystickId (joystick),
		driveFwdAxis (fwd), driveBwdAxis (bwd), turnLeftAxis (left), turnRightAxis (right)
	{
	}

	void CCarJoystickControls::Read (DriversInput& output) const
	{
		output =
		{
			-fminf (0.f, sf::Joystick::getAxisPosition (joystickId, driveFwdAxis) - joystickFwdAxisFault) / maxButtonPressure,
			-fmaxf (0.f, sf::Joystick::getAxisPosition (joystickId, driveBwdAxis) - joystickBwdAxisFault) / maxButtonPressure,
			fminf (0.f, sf::Joystick::getAxisPosition (joystickId, turnLeftAxis) - joystickLeftAxisFault) / maxButtonPressure,
			fmaxf (0.f, sf::Joystick::getAxisPosition (joystickId, turnRightAxis) - joystickRightAxisFault) / maxButtonPressure
		};
	}
}
