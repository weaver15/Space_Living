#pragma once

enum ControllerType
{
	Keyboard,
	Gamepad
};

class ControllerManager
{
public:

	ControllerManager();
	~ControllerManager();

	ControllerType GetControllerType();
	void SetControllerType(ControllerType type);

private:
	ControllerType CurrentControllerType;

};

