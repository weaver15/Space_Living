#pragma once
#include <SDL.h>
#include "AnimatedObject.h"
#include "InventoryItem.h"
#include "Font.h"

struct EquippedItems
{
	InventoryItem Head;
	InventoryItem Chest;
	InventoryItem Legs;
	InventoryItem Hands;
	InventoryItem Weapon1;
	InventoryItem Weapon2;
};

struct EquipmentSlotInformation
{
	SDL_Point HeadSlot;
	SDL_Point ChestSlot;
	SDL_Point LegsSlot;
	SDL_Point HandSlot;
	SDL_Point Weapon1Slot;
	SDL_Point Weapon2Slot;
};

enum EquippedTag
{
	HeadStat,
	ChestStat,
	FeetStat,
	HandStat,
	WeaponStat
};
static const std::string EquippedTagStrings[] = { "HeadStat", "ChestStat", "FeetStat", "HandStat", "WeaponStat" };



class EquipInventory : public AnimatedObject
{
public:
	EquipInventory();
	~EquipInventory();
	EquipInventory(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	InventoryItem EquipItem(SDL_Renderer* renderer, InventoryItem* item);
	void SetEquipmentSlots(EquipmentSlotInformation equipmentInfo);
	bool InitializeFont(SDL_Renderer* renderer);
	void UpdateDetails(SDL_Renderer* renderer);
private:
	EquippedItems strEquippedItems;
	SDL_Point HeadSlot;
	SDL_Point ChestSlot;
	SDL_Point LegsSlot;
	SDL_Point HandsSlot;
	SDL_Point Weapon1Slot;
	SDL_Point Weapon2Slot;
	Font EquippedFont;

};