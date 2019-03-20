#include "Inventory.h"

#pragma region Inventory
#pragma region Constructor/Deconstructor
Inventory::Inventory()
{
	vecInventoryItems.resize(0);
	SetIsAlive(true);
	SetTag("Inventory");
}
Inventory::~Inventory()
{
	vecInventoryItems.~vector();
	vecInventoryItems.resize(0);
	iMaxInventory = 0; 
	msSelector.~MenuSelector(); 
	pInventorySize = { 0, 0 };
}
Inventory::Inventory(InventoryInformation info)
	:AnimatedObject(info.TextureInfo, info.Location)
{
	SetIsAlive(true);
	pInventorySize = { 8,4 };
	iMaxInventory = pInventorySize.x * pInventorySize.y;
	vecInventoryItems.resize(iMaxInventory);
	SDL_Rect selectorLocation = info.Location;
	SDL_Rect equipmentLocation = { info.Location.x - 50, info.Location.y - 50};
	msSelector = MenuSelector(info.SelectorTextureInfo, selectorLocation, info.TotalRowCol, pInventorySize);
	invEquipment = EquipInventory(info.EquipmentTextureInfo, equipmentLocation, info.TotalRowCol);

	SetTag("Inventory");
	printf("Inventory: Inventory init\n", SDL_GetError());
}
#pragma endregion 

#pragma region Draw/Update
void Inventory::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	AnimatedObject::Draw(renderer, texture, camera);
	for each (InventoryItem item in vecInventoryItems)
	{
		if (item.IsAlive())
		{
			item.SetInventoryItemLocation(camera);
			item.Draw(renderer, texture, camera);
		}
	}
	invEquipment.Draw(renderer, texture, camera);
	msSelector.Draw(renderer, texture, camera);
	statTextBox.Draw(renderer, texture, camera);
}

void Inventory::Update()
{
}
void Inventory::UpdateInventoryLocation(SDL_Renderer* renderer, SDL_Rect* camera)
{
	SetLocation(camera->x + camera->w / 4 + iInventoryDrawOffSetX, camera->y + camera->h / 4 + iInventoryDrawOffSetY);
	invEquipment.SetLocation((camera->x + camera->w / 4) - iInventoryDrawOffSetX, (camera->y + camera->h / 4)- iInventoryDrawOffSetY);
	msSelector.SetLocation(camera);
	MoveStatTextBox(renderer);
}
#pragma endregion 

#pragma region Inventory Visibility 

void Inventory::ShowInventory()
{
	bIsInventoryShowing = true;
}
void Inventory::HideInventory()
{
	bIsInventoryShowing = false;
}

#pragma endregion 

#pragma region Selector
void Inventory::MoveSelector(Direction direction, SDL_Renderer* renderer)
{
	msSelector.MoveSelector(direction);
	MoveStatTextBox(renderer);
}
#pragma endregion 

#pragma region Items
void Inventory::AddItem(InventoryItem* item)
{
	if (iCurrentNumberOfItems < iMaxInventory)
	{
		SDL_Point slot = GetNextInventorySlot(); 
		SDL_Point slotLocation = GetNextInventorySlotLocation(slot);
		SDL_Rect location = { slotLocation.x + 4, slotLocation.y + 4, 34, 34 };
		item->SetLocation(&location);
		item->SetInventoryItemSlot(slot);
		vecInventoryItems[slot.x + (slot.y * 8)] = *item;
		iCurrentNumberOfItems = iCurrentNumberOfItems + 1;
	}
}

void Inventory::AddItem(InventoryItem item)
{
	if (iCurrentNumberOfItems < iMaxInventory)
	{
		SDL_Point slot = GetNextInventorySlot();
		SDL_Point slotLocation = GetNextInventorySlotLocation(slot);
		SDL_Rect location = { slotLocation.x + 4, slotLocation.y + 4, 34, 34 };
		item.SetLocation(&location);
		item.SetInventoryItemSlot(slot);
		vecInventoryItems[slot.x + (slot.y * 8)] = item;
		iCurrentNumberOfItems = iCurrentNumberOfItems + 1;
	}
}

SDL_Point Inventory::GetNextInventorySlot()
{
	SDL_Point slot = { 0,  0 };
	for (int i = 0; i < iMaxInventory; i++)
	{
		if (!vecInventoryItems[i].IsAlive())
		{
			int slotRow = i % pInventorySize.x;
			int slotCol = (int)(i / pInventorySize.x);
			slot = { slotRow,  slotCol };
			return slot;
		}
	}
	return slot; 
}

SDL_Point Inventory::GetNextInventorySlotLocation(SDL_Point slot)
{
	SDL_Point loc = { Location.x, Location.y };
	loc.x = loc.x + (slot.x * 38);
	loc.y = loc.y + (slot.y * 38);
	return loc; 
}

void Inventory::RemoveSelectedItem()
{
	int i = GetIndexForSelectedItem();
	if (i >= 0)
	{
		vecInventoryItems[i].SetIsAlive(false);
		iCurrentNumberOfItems = iCurrentNumberOfItems - 1;
	}
}
int Inventory::GetIndexForSelectedItem()
{
	auto selectedSlot = msSelector.GetSelectedSlot();
	for (int i = 0; i < vecInventoryItems.size(); i++)
	{
		if (vecInventoryItems[i].IsAlive())
		{
			auto itemSlot = vecInventoryItems[i].GetInventorySlot();
			if (itemSlot.x == selectedSlot.x && itemSlot.y == selectedSlot.y)
			{
				return i;
			}
		}

	}
	return -1; 
}
bool Inventory::EquipSelectedItem(SDL_Renderer* renderer, Stats* stats)
{
	int i = GetIndexForSelectedItem();
	if (i>=0)
	{
		InventoryItem unequip = invEquipment.EquipItem(renderer, &vecInventoryItems[i]);
		if (unequip.GetValue() > 0)
		{
			stats->iAttack -= unequip.GetAttack();
			stats->iDefese -= unequip.GetDefense();
			AddItem(unequip);
		}
		stats->iAttack += vecInventoryItems[i].GetAttack();
		stats->iDefese += vecInventoryItems[i].GetDefense();
		bool isWeapon = vecInventoryItems[i].GetInventoryType() == InventoryType::Weapon;
		
		vecInventoryItems[i].RemoveItem();	
		iCurrentNumberOfItems = iCurrentNumberOfItems - 1;
		MoveStatTextBox(renderer);

		return isWeapon;
	}
	return false; 
}

void Inventory::SetEquipmentSlots(EquipmentSlotInformation equipmentInfo)
{
	invEquipment.SetEquipmentSlots(equipmentInfo);
}

bool Inventory::InitializeFont(SDL_Renderer* renderer)
{
	invEquipment.InitializeFont(renderer);
	statTextBox.InitializeFont(renderer, 22, FontDialogBoxTag::Stat);
	return true; 
}
#pragma endregion 

#pragma region Stat Text Box
bool Inventory::InitializeStatTextBox(SDL_Rect textureInfo)
{
	statTextBox = FontDialogBox(std::pair<SDL_Rect, SDL_Rect> {textureInfo, *msSelector.GetLocation()});
	statTextBox.SetIsAlive(true);
	return true;
}

void Inventory::MoveStatTextBox(SDL_Renderer* renderer)
{
	SDL_Rect location = *msSelector.GetLocation();
	//Move the stat box a little to the right of the selector
	location.x += 45;
	location.y -= 25;
	statTextBox.SetLocation(&location);

	//Move the font to be inside the text box and update it to the current selected items value
	GetIndexForSelectedItem();
	location.x +=  5;
	location.y +=  5;
	statTextBox.UpdateLocation(&location, FontDialogBoxTag::Stat);
	int i = GetIndexForSelectedItem();

	if (i >= 0 && vecInventoryItems[i].GetValue() > 0)
	{
		statTextBox.UpdateFont(renderer, std::to_string(vecInventoryItems[i].GetValue()), FontDialogBoxTag::Stat);
	}
	else
	{
		statTextBox.UpdateFont(renderer, " ", FontDialogBoxTag::Stat);
	}

}

#pragma endregion 

#pragma endregion 





