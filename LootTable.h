#pragma once
#include <SDL.h>
#include <vector>
#include "InventoryItem.h"

class LootTable
{
public:
	LootTable();
	~LootTable();

	void AddItem(InventoryItem item);
	InventoryItem GetRandomItem();

private:
	std::vector<InventoryItem> vecItems;
};

