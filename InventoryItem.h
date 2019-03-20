#pragma once
#include <SDL.h>
#include "AnimatedObject.h"

enum WeaponType
{
	Pistol,
	Rifle
};

enum InventoryType
{
	Armor,
	Weapon
};

enum ArmorType
{
	Head,
	Chest,
	Legs,
	Hands
};


class InventoryItem : public AnimatedObject
{
public:
	InventoryItem();
	~InventoryItem();
	InventoryItem::InventoryItem(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol);

	void SetInventoryItemSlot(SDL_Point slot);
	void SetInventoryType(InventoryType type);
	void SetWeaponType(WeaponType type);
	void SetArmorType(ArmorType type);

	void SetInventoryItemLocation(SDL_Rect* camera); 
	void SetEquippedItemLocation(SDL_Rect* camera);
	void SetEquipped(bool isEquipped) { bIsEquipped = isEquipped; }
	void SetStats(int currentLevel);

	SDL_Point GetInventorySlot() const { return pInventorySlot; }
	InventoryType GetInventoryType() const {return enmInventoryType; }
	WeaponType GetWeaponType() const { return enmWeaponType; }
	ArmorType GetArmorType() const { return enmArmorType; }

	bool IsEquipped() const { return bIsEquipped;  }
	int GetAttack() const { return iAttack;  }
	int GetDefense() const { return iDefense; }
	int GetValue();

	void RemoveItem();
private:
	SDL_Point pInventorySlot; 
	InventoryType enmInventoryType; 
	WeaponType enmWeaponType;
	ArmorType enmArmorType; 
	int iDefense = 0;
	int iAttack = 0;
	int iDrawPaddding = 4;
	bool bIsEquipped = false; 
};


