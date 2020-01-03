#include "GameInfoManager.h"



GameInfoManager::GameInfoManager()
{
	pugi::xml_parse_result result = Doc.load_file("GameInfo.xml");
	char levelToLoad[] = "1";
	Level = Doc.find_child_by_attribute("Level", "stage", levelToLoad);
}

GameInfoManager::~GameInfoManager()
{
}

int GameInfoManager::LoadTileSize()
{
	pugi::xml_node tile_details = Doc.child("Tile");
	int size = tile_details.attribute("size").as_int();
	TileSize = size; 
	return size;
}

const int GameInfoManager::GetTileSize()
{
	return TileSize; 
}

std::vector<SDL_Rect> GameInfoManager::LoadTileTextureLocations()
{
	pugi::xml_node tile_details = Doc.child("Tile");
	int tileSize = LoadTileSize();
	std::vector<SDL_Rect> vecTileTextureLocations;

	pugi::xml_node blackTile = tile_details.child("tile_black");
	SDL_Rect textureLocation = { blackTile.attribute("x").as_int(),
		blackTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node noWallTile = tile_details.child("tile_n");
	textureLocation = { noWallTile.attribute("x").as_int(),
		noWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node topWallTile = tile_details.child("tile_t");
	textureLocation = { topWallTile.attribute("x").as_int(),
		topWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node rightWallTile = tile_details.child("tile_r");
	textureLocation = { rightWallTile.attribute("x").as_int(),
		rightWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node bottomWallTile = tile_details.child("tile_b");
	textureLocation = { bottomWallTile.attribute("x").as_int(),
		bottomWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node leftWallTile = tile_details.child("tile_l");
	textureLocation = { leftWallTile.attribute("x").as_int(),
		leftWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node topRightWallTile = tile_details.child("tile_tr");
	textureLocation = { topRightWallTile.attribute("x").as_int(),
		topRightWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node bottomRightWallTile = tile_details.child("tile_br");
	textureLocation = { bottomRightWallTile.attribute("x").as_int(),
		bottomRightWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node bottomLeftWallTile = tile_details.child("tile_bl");
	textureLocation = { bottomLeftWallTile.attribute("x").as_int(),
		bottomLeftWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node topLeftWallTile = tile_details.child("tile_tl");
	textureLocation = { topLeftWallTile.attribute("x").as_int(),
		topLeftWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node topBottomWallTile = tile_details.child("tile_tb");
	textureLocation = { topBottomWallTile.attribute("x").as_int(),
		topBottomWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	pugi::xml_node leftRightWallTile = tile_details.child("tile_lr");
	textureLocation = { leftRightWallTile.attribute("x").as_int(),
		leftRightWallTile.attribute("y").as_int(),tileSize,tileSize };
	vecTileTextureLocations.push_back(textureLocation);

	return vecTileTextureLocations;
}

std::vector<int> GameInfoManager::LoadTileLayout()
{
	pugi::xml_node tileNode = Level.child("Tiles");
	std::string tileLayout = tileNode.child("tile_types").attribute("id").as_string();
	std::stringstream ss(tileLayout);
	std::vector<int> tileTypes;
	int value;
	while (ss >> value)
	{
		tileTypes.push_back(value);

		if (ss.peek() == ',')
			ss.ignore();
	}

	return tileTypes;
}

void GameInfoManager::LoadGrid(Grid* grid)
{
	ScreenTileSize = Level.child("Tiles").attribute("size").as_int();
	*(grid) = Grid(ScreenTileSize, ScreenTileSize);

	auto vecTileTextureLocations = LoadTileTextureLocations();
	pugi::xml_node tileNode = Level.child("Tiles");
	int totalTiles = tileNode.attribute("count").as_int();
	ScreenTileSize = tileNode.attribute("size").as_int();
	int x = 0, y = 0, grid_x = 0, grid_y = 0;
	std::vector<Tile> tiles;
	tiles.resize(totalTiles);
	TileDetails details;
	SDL_Rect loc = { x, y, TileSize,TileSize };
	auto vecTileType = LoadTileLayout();
	for (int i = 0; i < totalTiles; i++)
	{

		loc.x = x;
		loc.y = y;
		details.TileWalls = (WallLocation)vecTileType[i];
		details.TextureLocation = vecTileTextureLocations[vecTileType[i]-1];
		details.TileLocation = loc;
		details.GridLoc = { grid_x, grid_y };
		details.TileType = vecTileType[i];
		Tile tile = Tile(details);
		tiles[i] = tile;
		x += TileSize;
		grid_x += 1;
		if ((i + 1) % ScreenTileSize == 0)
		{
			x = 0;
			y += TileSize;
			grid_x = 0;
			grid_y += 1;
		}
	}

	grid->SetTiles(tiles);
}

void GameInfoManager::LoadLevelInformation(SDL_Point* level)
{
	pugi::xml_node levelSize = Level.child("LevelSize");
	level->x = levelSize.attribute("width").as_int();
	level->y = levelSize.attribute("height").as_int();

	//Dialogue
	pugi::xml_node dialogue = Doc.child("Dialogue");
	int count = dialogue.attribute("count").as_int();
	//*(doorManager) = DoorManager(numberOfDoors);
	for (const auto& node : dialogue.children("Text"))
	{
		std::string value = node.attribute("value").as_string();
		int id = node.attribute("id").as_int();
		
	}

}

void GameInfoManager::LoadPlayerInformation(Player* player)
{
#ifdef _DEBUG
	printf("Loading Player Information\n", SDL_GetError());
#endif

	pugi::xml_node playerInfo = Level.child("PlayerInfo");
	SDL_Rect location;
	location.h = playerInfo.child("Location").attribute("height").as_int();
	location.w = playerInfo.child("Location").attribute("width").as_int();
	location.x = playerInfo.child("Location").attribute("x").as_int();
	location.y = playerInfo.child("Location").attribute("y").as_int();
	SDL_Rect textureInformation;
	textureInformation.h = playerInfo.child("Texture").attribute("height").as_int();
	textureInformation.w = playerInfo.child("Texture").attribute("width").as_int();
	textureInformation.x = playerInfo.child("Texture").attribute("x").as_int();
	textureInformation.y = playerInfo.child("Texture").attribute("y").as_int();
	
	//this will eventually be used for animated frames
	SDL_Point totalRowCol;
	totalRowCol.x = playerInfo.child("Texture").attribute("row").as_int();
	totalRowCol.y = playerInfo.child("Texture").attribute("column").as_int();
	
	*player = Player(std::pair<SDL_Rect, SDL_Rect>{ textureInformation, location});
}
 
void GameInfoManager::LoadBossInformation(Boss* boss)
{
#ifdef _DEBUG
	printf("Loading Boss Information\n", SDL_GetError());
#endif

	pugi::xml_node bossInfo = Level.child("BossInfo");

	int width = bossInfo.child("Texture").attribute("width").as_int();
	int height = bossInfo.child("Texture").attribute("height").as_int();
	SDL_Rect bossTextureInformation{
		bossInfo.child("Texture").attribute("x").as_int(),
		bossInfo.child("Texture").attribute("y").as_int(),
		width,
		height
	};

	SDL_Rect bossLocation
	{
		bossInfo.child("Location").attribute("x").as_int()* TileSize,
		bossInfo.child("Location").attribute("y").as_int()* TileSize,
		width,
		height
	};
	*boss = Boss(std::pair<SDL_Rect, SDL_Rect>{ bossTextureInformation, bossLocation});
}

void GameInfoManager::LoadDialogBoxInformation(FontDialogBox* dialogBox)
{
	SDL_Rect boxInformation;
	pugi::xml_node dialog = Doc.child("Dialog");
	boxInformation.h = dialog.child("Box").attribute("height").as_int();
	boxInformation.w = dialog.child("Box").attribute("width").as_int();
	boxInformation.x = dialog.child("Box").attribute("x").as_int();
	boxInformation.y = dialog.child("Box").attribute("y").as_int();
	SDL_Rect location = { 0, 0,boxInformation.h, boxInformation.w };
	*(dialogBox) = FontDialogBox(std::pair<SDL_Rect, SDL_Rect>{ boxInformation, location});
}

void GameInfoManager::LoadSolidObjects(SolidObjectManager* manager)
{
	pugi::xml_node solids = Level.child("Solids");
	pugi::xml_node solidTextures = solids.child("Textures");
	std::vector<SDL_Rect> textures;
	for (const auto& node : solidTextures.children("Texture"))
	{
		int height = node.attribute("height").as_int();
		int width = node.attribute("width").as_int();
		int x = node.attribute("x").as_int();
		int y = node.attribute("y").as_int();
		SDL_Rect solidTexture = { x,y,width,height };
		textures.push_back(SDL_Rect{ x,y,width,height });
	}

	pugi::xml_node solidLocations = solids.child("Locations");
	int numberOfObjects = solidLocations.attribute("count").as_int();
	//_SolidObjectManager = SolidObjectManager();
	vector<pair<SDL_Rect, SDL_Rect>> solidObjects;
	for (const auto& node : solidLocations.children("Object"))
	{
		int x = node.attribute("x").as_int();
		int y = node.attribute("y").as_int();
		int texture = node.attribute("texture").as_int();
		SDL_Rect location = { x,y ,textures[texture].w, textures[texture].h };
		solidObjects.push_back(pair<SDL_Rect, SDL_Rect>{textures[texture], location});
		//_SolidObjectManager.AddObject(textures[texture], location);
	}
	*(manager) = SolidObjectManager(solidObjects);
}

void GameInfoManager::LoadDoorInformation(DoorManager* doorManager)
{
	pugi::xml_node doors = Level.child("Doors");
	//Vert Closed Door texture
	SDL_Rect closedDoorTexturVert{
		doors.child("ClosedTexture").child("Vertical").attribute("x").as_int(),
		doors.child("ClosedTexture").child("Vertical").attribute("y").as_int(),
		doors.child("ClosedTexture").child("Vertical").attribute("width").as_int(),
		doors.child("ClosedTexture").child("Vertical").attribute("height").as_int()
	};

	SDL_Rect closedDoorTexturHoriz{
		doors.child("ClosedTexture").child("Horizontal").attribute("x").as_int(),
		doors.child("ClosedTexture").child("Horizontal").attribute("y").as_int(),
		doors.child("ClosedTexture").child("Horizontal").attribute("width").as_int(),
		doors.child("ClosedTexture").child("Horizontal").attribute("height").as_int()
	};

	SDL_Rect openDoorTextureVert{
		doors.child("OpenTexture").child("Vertical").attribute("x").as_int(),
		doors.child("OpenTexture").child("Vertical").attribute("y").as_int(),
		doors.child("OpenTexture").child("Vertical").attribute("width").as_int(),
		doors.child("OpenTexture").child("Vertical").attribute("height").as_int()
	};

	SDL_Rect openDoorTextureHoriz{
		doors.child("OpenTexture").child("Horizontal").attribute("x").as_int(),
		doors.child("OpenTexture").child("Horizontal").attribute("y").as_int(),
		doors.child("OpenTexture").child("Horizontal").attribute("width").as_int(),
		doors.child("OpenTexture").child("Horizontal").attribute("height").as_int()
	};

	SDL_Rect bossDoorTextureVert{
		doors.child("BossDoorTexture").child("Vertical").attribute("x").as_int(),
		doors.child("BossDoorTexture").child("Vertical").attribute("y").as_int(),
		doors.child("BossDoorTexture").child("Vertical").attribute("width").as_int(),
		doors.child("BossDoorTexture").child("Vertical").attribute("height").as_int()
	};

	SDL_Rect bossDoorTextureHoriz{
		doors.child("BossDoorTexture").child("Horizontal").attribute("x").as_int(),
		doors.child("BossDoorTexture").child("Horizontal").attribute("y").as_int(),
		doors.child("BossDoorTexture").child("Horizontal").attribute("width").as_int(),
		doors.child("BossDoorTexture").child("Horizontal").attribute("height").as_int()
	};

	pugi::xml_node doorsLocationInfo = doors.child("Locations");
	int numberOfDoors = doorsLocationInfo.attribute("count").as_int();
	*(doorManager) = DoorManager(numberOfDoors);
	doorManager->SetTextureLocations(closedDoorTexturVert, openDoorTextureVert,
		closedDoorTexturHoriz, openDoorTextureHoriz);

	int xOffset = 10;
	int yOffset = 20;
	for (const auto& node : doorsLocationInfo.children("Door"))
	{
		std::string direction = node.attribute("direction").as_string();
		bool isVertical = direction == "vertical";
		if (!isVertical)
		{
			int width = closedDoorTexturHoriz.w;
			int height = closedDoorTexturHoriz.h;
			int x = (node.attribute("x").as_int() - 1) * TileSize;
			int y = node.attribute("y").as_int() * TileSize - yOffset;
			SDL_Rect doorLocation = { x,y,width,height };
			int id = node.attribute("id").as_int();
			Door door = Door{ id, closedDoorTexturHoriz, doorLocation, isVertical };
			doorManager->AddDoor(door);
		}
		else
		{
			int width = closedDoorTexturVert.w;
			int height = closedDoorTexturVert.h;
			int x = node.attribute("x").as_int() * TileSize - xOffset;
			int y = (node.attribute("y").as_int() - 1) * TileSize;
			SDL_Rect doorLocation = { x,y,width,height };
			int id = node.attribute("id").as_int();
			Door door = Door{ id, closedDoorTexturVert, doorLocation, isVertical };
			doorManager->AddDoor(door);
		}

	}

	pugi::xml_node bossDoorLocationInfo = doors.child("BossDoorLocation");
	std::string direction = bossDoorLocationInfo.attribute("direction").as_string();
	bool isVertical = direction == "vertical";

	if (!isVertical)
	{
		int width = bossDoorTextureHoriz.w;
		int height = bossDoorTextureHoriz.h;
		int bossDoorX = (bossDoorLocationInfo.attribute("x").as_int() - 1) * TileSize;
		int bossDoorY = bossDoorLocationInfo.attribute("y").as_int() * TileSize - yOffset;
		SDL_Rect bossDoorLocation = { bossDoorX,bossDoorY,width,height };
		int id = bossDoorLocationInfo.attribute("id").as_int();
		Door door = Door{ id, bossDoorTextureHoriz, bossDoorLocation, isVertical, true };
		doorManager->AddDoor(door);
	}
	else
	{
		int width = bossDoorTextureVert.w;
		int height = bossDoorTextureVert.h;
		int bossDoorX = bossDoorLocationInfo.attribute("x").as_int() * TileSize - xOffset;
		int bossDoorY = (bossDoorLocationInfo.attribute("y").as_int() - 1) * TileSize;
		SDL_Rect bossDoorLocation = { bossDoorX,bossDoorY,width,height };
		int id = bossDoorLocationInfo.attribute("id").as_int();
		Door door = Door{ id, bossDoorTextureVert, bossDoorLocation, isVertical, true };
		doorManager->AddDoor(door);
	}
}

void GameInfoManager::LoadProjectileInformation(Player* player, Boss* boss)
{
	pugi::xml_node projectile = Doc.child("Projectile");
	SDL_Rect projectileTextureInformation;
	projectileTextureInformation.h = projectile.child("Texture").attribute("height").as_int();
	projectileTextureInformation.w = projectile.child("Texture").attribute("width").as_int();
	projectileTextureInformation.x = projectile.child("Texture").attribute("x").as_int();
	projectileTextureInformation.y = projectile.child("Texture").attribute("y").as_int();
	SDL_Point projectileRowCol;
	projectileRowCol.x = projectile.child("Texture").attribute("row").as_int();
	projectileRowCol.y = projectile.child("Texture").attribute("column").as_int();
	int maxNumberOfprojectiles = projectile.child("max_projectiles_on_screen").attribute("count").as_int();

	player->InitialIzeProjectile(&projectileTextureInformation, &projectileRowCol, maxNumberOfprojectiles);

	boss->InitialIzeProjectile(&projectileTextureInformation, &projectileRowCol, maxNumberOfprojectiles);
}


void GameInfoManager::LoadEnemyInformation(EnemyManager* enemyManager, SpawnPointManager* spawnManager)
{
	pugi::xml_node enemy = Level.child("EnemyInfo");
	*(enemyManager) = EnemyManager(enemy.child("NumberOfEnemies").attribute("count").as_int());

	pugi::xml_node spawns = enemy.child("Spawns");

	SDL_Rect spawnTextureInformation{
		spawns.child("SpawnTexture").attribute("x").as_int(),
		spawns.child("SpawnTexture").attribute("y").as_int(),
		spawns.child("SpawnTexture").attribute("width").as_int(),
		spawns.child("SpawnTexture").attribute("height").as_int()
	};

	pugi::xml_node spawnLocationInfo = spawns.child("Locations");
	int numberOfSpawns = spawnLocationInfo.attribute("count").as_int();
	*(spawnManager) = SpawnPointManager(numberOfSpawns);
	for each (pugi::xml_node node in spawnLocationInfo.children("Spawn"))
	{
		//int x = node.attribute("x").as_int();
		//int y = node.attribute("y").as_int();
		int x = (node.attribute("x").as_int() - 1) * TileSize;
		int y = node.attribute("y").as_int() * TileSize;
		int width = spawnTextureInformation.w;
		int height = spawnTextureInformation.h;
		int rate = node.attribute("rate").as_int();
		int door = node.attribute("DoorId").as_int();
		SDL_Rect locationInformation = { x,y,width,height };
		spawnManager->AddSpawnPoints(spawnTextureInformation, locationInformation, rate, door);
	}
}
