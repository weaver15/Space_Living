#pragma once
#include "Font.h"

enum HUDTag
{
	PlayerHitPointsLable,
	PlayerHitPointsCount,
};
static const std::string HUDTagStrings[] = {"Health", "PlayerHitPointsCount"};

class HUD
{
public:
	HUD();
	~HUD();

	void Draw(SDL_Renderer* renderer);
	void InitializeFont(SDL_Renderer* renderer, int hitpoints);;
	bool UpdatePlayerHitPoints(SDL_Renderer* renderer, int hitPoints);
	void SetStartOfLevelHUD(SDL_Renderer* renderer, int hitPoints);
private:
	Font HUDFont;

};

