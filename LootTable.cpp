#include "LootTable.h"



LootTable::LootTable()
{
	vecItems = std::vector<InventoryItem>();
}


LootTable::~LootTable()
{
}

void LootTable::AddItem(InventoryItem item)
{
	vecItems.push_back(item);
}

InventoryItem LootTable::GetRandomItem()
{
	int index = rand() % vecItems.size(); 

	return vecItems[index];
}
