#pragma once
#include <vector>
#include <SDL.h>

#include "AnimatedObject.h"
#include "InventoryItem.h"
#include "Texture.h"
#include "MenuSelector.h"
#include "Font.h"
#include "EquipInventory.h"
#include "FontDialogBox.h"


struct Stats
{
	int iAttack = 0;
	int iDefese = 0;
};

struct InventoryInformation
{
	SDL_Rect TextureInfo;
	SDL_Rect Location;
	SDL_Point TotalRowCol;
	SDL_Rect SelectorTextureInfo;
	SDL_Rect EquipmentTextureInfo;
	SDL_Rect TextBoxTextureInfo;
};

class Inventory: public AnimatedObject
{
public:
	Inventory();
	~Inventory();
	Inventory(InventoryInformation info);

	void Update();
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	void ShowInventory();
	void HideInventory();
	void MoveSelector(Direction direction, SDL_Renderer* renderer);

	bool EquipSelectedItem(SDL_Renderer* renderer, Stats* stats);
	void RemoveSelectedItem();
	int GetIndexForSelectedItem();

	void AddItem(InventoryItem* item); 
	void AddItem(InventoryItem item);
	SDL_Point GetNextInventorySlotLocation(SDL_Point slot); 
	SDL_Point GetNextInventorySlot();
	void UpdateInventoryLocation(SDL_Renderer* renderer, SDL_Rect* camera);
	void SetEquipmentSlots(EquipmentSlotInformation equipmentInfo);
	bool IsInventoryShowing() const { return bIsInventoryShowing;};
	bool InitializeFont(SDL_Renderer* renderer);

	//Stat text box
	void MoveStatTextBox(SDL_Renderer* renderer);
	bool InitializeStatTextBox(SDL_Rect textureInfo);

private:
	int iMaxInventory = 0;
	int iCurrentNumberOfItems = 0;
	int iFontSize = 22;
	int iItemTextureSize = 38;
	int iInventoryDrawOffSetX = 50;
	int iInventoryDrawOffSetY = 75;
	bool bIsInventoryShowing = false; 
	std::vector<InventoryItem> vecInventoryItems;
	MenuSelector msSelector; 
	SDL_Point pInventorySize; 
	EquipInventory invEquipment; 
	FontDialogBox statTextBox;

	
};

