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


bool HUD::UpdatePlayerHitPoints(SDL_Renderer* renderer, int hitPoints)
{
	return HUDFont.UpdateText(renderer, std::to_string(hitPoints), HUDTagStrings[HUDTag::PlayerHitPointsCount]);
}

void HUD::SetStartOfLevelHUD(SDL_Renderer* renderer, int hitPoints)
{
	HUDFont.UpdateText(renderer, std::to_string(hitPoints), HUDTagStrings[HUDTag::PlayerHitPointsCount]);
}

void HUD::InitializeFont(SDL_Renderer* renderer, int hitpoints)
{
	//Health Label
	HUDFont.LoadFont(renderer);
	SDL_Color textColor = { 0, 255, 0 };

	SDL_Rect hpLocation = { (640 / 8) - 50, (480 / 8) - 50, 0, 0 };
	HUDFont.AddText(renderer, HUDTagStrings[HUDTag::PlayerHitPointsLable] + ": ", &hpLocation, HUDTagStrings[HUDTag::PlayerHitPointsLable], textColor);

	//Health Count
	SDL_Rect hpCountLocation = { (640 / 8) + 50, (480 / 8) - 50, 0, 0 };
	SDL_Color hpCountTextColor = { 200, 0, 0 };
	HUDFont.AddText(renderer, std::to_string(hitpoints), &hpCountLocation, HUDTagStrings[HUDTag::PlayerHitPointsCount], textColor);

}
