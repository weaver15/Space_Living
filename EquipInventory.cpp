
#include "EquipInventory.h"

#pragma region Equip Inventory
#pragma region Constructor/Deconstructor
EquipInventory::EquipInventory()
{
	SetTag("EquipInventory");
}
EquipInventory::~EquipInventory()
{

}
EquipInventory::EquipInventory(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol)
	:AnimatedObject(textureInfo, location, totalRowCol)
{
	SetIsAlive(true);
	SetTag("EquipInventory");
	printf("EquipInventory: Equipment Inventory init\n", SDL_GetError());
}
#pragma endregion 

void EquipInventory::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	AnimatedObject::Draw(renderer, texture, camera);
	if (strEquippedItems.Head.IsEquipped())
	{
		strEquippedItems.Head.SetLocation(GetLocation(), &HeadSlot);
		strEquippedItems.Head.Draw(renderer, texture, camera);
	}
	if (strEquippedItems.Chest.IsEquipped())
	{
		strEquippedItems.Chest.SetLocation(GetLocation(), &ChestSlot);
		strEquippedItems.Chest.Draw(renderer, texture, camera);
	}
	if (strEquippedItems.Legs.IsEquipped())
	{
		strEquippedItems.Legs.SetLocation(GetLocation(), &LegsSlot);
		strEquippedItems.Legs.Draw(renderer, texture, camera);
	}

	if (strEquippedItems.Hands.IsEquipped())
	{
		strEquippedItems.Hands.SetLocation(GetLocation(), &HandsSlot);
		strEquippedItems.Hands.Draw(renderer, texture, camera);
	}

	if (strEquippedItems.Weapon1.IsEquipped())
	{
		strEquippedItems.Weapon1.SetLocation(GetLocation(), &Weapon1Slot);
		strEquippedItems.Weapon1.Draw(renderer, texture, camera);
	}

	EquippedFont.RenderText(renderer);
}

InventoryItem EquipInventory::EquipItem(SDL_Renderer* renderer, InventoryItem* item)
{
	InventoryItem unequip;
	switch (item->GetInventoryType())
	{
	case InventoryType::Weapon:
		if (strEquippedItems.Weapon1.IsEquipped())
			unequip = strEquippedItems.Weapon1;

		strEquippedItems.Weapon1 = *item;
		strEquippedItems.Weapon1.SetEquipped(true);
		strEquippedItems.Weapon1.SetLocation(GetLocation(), &Weapon1Slot);
		break;
	case InventoryType::Armor:
		ArmorType type = item->GetArmorType();

		if (type == ArmorType::Chest)
		{
			if (strEquippedItems.Chest.IsEquipped())
				unequip = strEquippedItems.Chest;

			strEquippedItems.Chest = *item;
			strEquippedItems.Chest.SetEquipped(true);
			strEquippedItems.Chest.SetLocation(GetLocation(), &ChestSlot);
		}
		else if (type == ArmorType::Head)
		{
			if (strEquippedItems.Head.IsEquipped())
				unequip = strEquippedItems.Head;

			strEquippedItems.Head = *item;
			strEquippedItems.Head.SetEquipped(true);
			strEquippedItems.Head.SetLocation(GetLocation(), &HeadSlot);
		}
		else if (type == ArmorType::Legs)
		{
			if (strEquippedItems.Legs.IsEquipped())
				unequip = strEquippedItems.Legs;

			strEquippedItems.Legs = *item;
			strEquippedItems.Legs.SetEquipped(true);
		}
		else if (type == ArmorType::Hands)
		{
			if (strEquippedItems.Hands.IsEquipped())
				unequip = strEquippedItems.Hands;

			strEquippedItems.Hands = *item;
			strEquippedItems.Hands.SetEquipped(true);
		}
		break;

	}
	UpdateDetails(renderer);
	return unequip;
}

void EquipInventory::SetEquipmentSlots(EquipmentSlotInformation equipmentInfo)
{
	HeadSlot = equipmentInfo.HeadSlot;
	ChestSlot = equipmentInfo.ChestSlot;
	LegsSlot = equipmentInfo.LegsSlot;
	HandsSlot = equipmentInfo.HandSlot;
	Weapon1Slot = equipmentInfo.Weapon1Slot;
	Weapon2Slot = equipmentInfo.Weapon2Slot;
}

bool EquipInventory::InitializeFont(SDL_Renderer* renderer)
{
	EquippedFont.LoadFont(renderer, 18);
	SDL_Color textColor = { 0, 0, 0 };

	//Head stat
	SDL_Rect location = { (640 / 8) + 210, (480 / 8) - 15, 0, 0 };
	EquippedFont.AddText(renderer, "0", &location, EquippedTagStrings[EquippedTag::HeadStat], textColor);

	////Chest stat
	location.y += 20;
	EquippedFont.AddText(renderer, "0", &location, EquippedTagStrings[EquippedTag::ChestStat], textColor);

	////Leg stat
	location.y += 20;
	EquippedFont.AddText(renderer, "0", &location, EquippedTagStrings[EquippedTag::FeetStat], textColor);

	////Hands stat
	location.y += 20;
	EquippedFont.AddText(renderer, "0", &location, EquippedTagStrings[EquippedTag::HandStat], textColor);

	////Weapon stat
	location.y += 20;
	EquippedFont.AddText(renderer, "0", &location, EquippedTagStrings[EquippedTag::WeaponStat], textColor);

	return true;
}

void EquipInventory::UpdateDetails(SDL_Renderer* renderer)
{
	EquippedFont.UpdateText(renderer, std::to_string(strEquippedItems.Head.GetDefense()), EquippedTagStrings[EquippedTag::HeadStat]);
	EquippedFont.UpdateText(renderer, std::to_string(strEquippedItems.Chest.GetDefense()), EquippedTagStrings[EquippedTag::ChestStat]);
	EquippedFont.UpdateText(renderer, std::to_string(strEquippedItems.Legs.GetDefense()), EquippedTagStrings[EquippedTag::FeetStat]);
	EquippedFont.UpdateText(renderer, std::to_string(strEquippedItems.Hands.GetDefense()), EquippedTagStrings[EquippedTag::HandStat]);
	EquippedFont.UpdateText(renderer, std::to_string(strEquippedItems.Weapon1.GetAttack()), EquippedTagStrings[EquippedTag::WeaponStat]);
}

#pragma endregion 