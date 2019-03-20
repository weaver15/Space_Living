#pragma once
#include "Font.h"

enum HUDTag
{
	EnergyLable,
	EnergyCount,
	PlayerHitPointsLable,
	PlayerHitPointsCount,
	AmmoLable,
	AmmoCount
};
static const std::string HUDTagStrings[] = { "Energy", "EnergyCount", "Health", "PlayerHitPointsCount", "AmmoLable", "AmmoCount" };

class HUD
{
public:
	HUD();
	~HUD();

	void Draw(SDL_Renderer* renderer);
	bool InitializeFont(SDL_Renderer* renderer, int hitpoints ,int startingEnergy);
	bool UpdatePlayerEnergy(SDL_Renderer* renderer, int energyCount);
	bool UpdatePlayerHitPoints(SDL_Renderer* renderer, int hitPoints);
	void SetStartOfLevelHUD(SDL_Renderer* renderer, int hitPoints, int energyCount);
private:
	Font HUDFont;

};

