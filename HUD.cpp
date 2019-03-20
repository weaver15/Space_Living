#include "HUD.h"


HUD::HUD()
{
}


HUD::~HUD()
{
}

void HUD::Draw(SDL_Renderer* renderer)
{
	HUDFont.RenderText(renderer);
}

bool HUD::UpdatePlayerEnergy(SDL_Renderer* renderer, int energyCount)
{
	return HUDFont.UpdateText(renderer, std::to_string(energyCount), HUDTagStrings[HUDTag::EnergyCount]);
}

bool HUD::UpdatePlayerHitPoints(SDL_Renderer* renderer, int hitPoints)
{
	return HUDFont.UpdateText(renderer, std::to_string(hitPoints), HUDTagStrings[HUDTag::PlayerHitPointsCount]);
}

void HUD::SetStartOfLevelHUD(SDL_Renderer* renderer, int hitPoints, int energyCount)
{
	HUDFont.UpdateText(renderer, std::to_string(energyCount), HUDTagStrings[HUDTag::EnergyCount]);
	HUDFont.UpdateText(renderer, std::to_string(hitPoints), HUDTagStrings[HUDTag::PlayerHitPointsCount]);
}

bool HUD::InitializeFont(SDL_Renderer* renderer, int hitpoints,  int startingEnergy)
{
	//Health Label
	HUDFont.LoadFont(renderer);
	SDL_Color textColor = { 0, 255, 0 };

	SDL_Rect hpLocation = { (640 / 8) - 50, (480 / 8) - 50, 0, 0 };
	HUDFont.AddText(renderer, HUDTagStrings[HUDTag::PlayerHitPointsLable] + ": ", &hpLocation, HUDTagStrings[HUDTag::EnergyLable], textColor);

	//Health Count
	SDL_Rect hpCountLocation = { (640 / 8) + 50, (480 / 8) - 50, 0, 0 };
	SDL_Color hpCountTextColor = { 200, 0, 0 };
	HUDFont.AddText(renderer, std::to_string(hitpoints), &hpCountLocation, HUDTagStrings[HUDTag::PlayerHitPointsCount], textColor);

	//Energy Label
	HUDFont.LoadFont(renderer);
	SDL_Rect location = { (640 / 8) - 50, (480 / 8), 0, 0 };
	HUDFont.AddText(renderer, HUDTagStrings[HUDTag::EnergyLable] + ": ", &location, HUDTagStrings[HUDTag::EnergyLable], textColor);

	//Energy Count
	SDL_Rect countLocation = { (640 / 8) + 50, (480 / 8), 0, 0 };
	SDL_Color countTextColor = { 0, 200, 0 };
	HUDFont.AddText(renderer, std::to_string(startingEnergy), &countLocation, HUDTagStrings[HUDTag::EnergyCount], textColor);

	return true; 
}
