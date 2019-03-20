#include "InventoryItem.h"


#pragma region Constructor/Deconstructor
InventoryItem::InventoryItem()
{
	SetTag("InventoryItem");
}


InventoryItem::~InventoryItem()
{
}

InventoryItem::InventoryItem(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol)
	:AnimatedObject(textureInfo, location, totalRowCol)
{
	SetIsAlive(true);
	SetTag("InventoryItem");
}
#pragma endregion

void InventoryItem::SetInventoryItemSlot(SDL_Point slot)
{
	pInventorySlot = slot;
}

void InventoryItem::SetInventoryType(InventoryType type)
{
	enmInventoryType = type;
}

void InventoryItem::SetWeaponType(WeaponType type)
{
	enmWeaponType = type; 
}

void InventoryItem::SetArmorType(ArmorType type)
{
	enmArmorType = type; 
}

void InventoryItem::SetInventoryItemLocation(SDL_Rect* camera)
{
	AnimatedObject::SetLocation(
		(camera->x + camera->w / 4) + ((pInventorySlot.x) * (Location.w + iDrawPaddding)) + iDrawPaddding + 50,
		(camera->y + camera->h / 4) + ((pInventorySlot.y) * (Location.h + iDrawPaddding))+ iDrawPaddding + 75);
}

void InventoryItem::SetEquippedItemLocation(SDL_Rect* camera)
{
	AnimatedObject::SetLocation(
		camera->x + Location.x + iDrawPaddding,
		camera->y + Location.y);
}

void InventoryItem::SetStats(int currentLevel)
{

	if (enmInventoryType == InventoryType::Armor)
	{
		iDefense = rand() % 10 + (currentLevel * 2);
		iAttack = 0;
	}
	else if (enmInventoryType == InventoryType::Weapon)
	{
		iAttack = rand() % 10 + (currentLevel * 2);
		iDefense = 0;
	}

}

int InventoryItem::GetValue()
{
	if (iAttack > 0)
		return iAttack;
	else if(iDefense > 0)
		return iDefense;
	else
		return NULL; 
}

void InventoryItem::RemoveItem()
{
	iDefense = NULL;
	iAttack = NULL;
	SetIsAlive(false);
	pInventorySlot = SDL_Point();
}