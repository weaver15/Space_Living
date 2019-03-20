#include "ControllerManager.h"



ControllerManager::ControllerManager()
{
}


ControllerManager::~ControllerManager()
{
}

ControllerType ControllerManager::GetControllerType()
{
	return CurrentControllerType;
}

void ControllerManager::SetControllerType(ControllerType type)
{
	CurrentControllerType = type; 
}