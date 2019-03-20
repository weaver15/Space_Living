#include "Grid.h"



Grid::Grid()
{
}

Grid::Grid(int width, int height)
{
	Width = width;
	Height = height;
	DIRS = { tuple<Direction, GridLocation>{Direction::Right, GridLocation{ 1, 0 }},
		tuple<Direction, GridLocation>{Direction::Left,GridLocation{ -1, 0 }},
		tuple<Direction, GridLocation>{Direction::Down, GridLocation{ 0, 1 }},
		tuple<Direction, GridLocation>{Direction::Up, GridLocation{ 0, -1 }} };
}



Grid::~Grid()
{
}

bool Grid::InBounds(GridLocation id)
{
	int x, y;
	tie(x, y) = id;
	return 0 <= x && x < Width && 0 <= y && y < Height;
}

bool Grid::IsPassable(Direction dir, GridLocation* id, Tile current)
{
	if (Impassable.count(*id))
		return false;

	if (dir == Direction::Left)
	{
		if (LeftWalls.count(current.GetGridLocation()))
			return false;
		if (RightWalls.count(*id))
			return false; 
	}
	else if (dir == Direction::Right)
	{
		if(RightWalls.count(current.GetGridLocation()))
			return false;
		if (LeftWalls.count(*id))
			return false;
	} 
		
	else if (dir == Direction::Up)
	{
		if (TopWalls.count(current.GetGridLocation()))
			return false;
		if (BottomWalls.count(*id))
			return false;
	}
	else if (dir == Direction::Down)
	{
		if(BottomWalls.count(current.GetGridLocation()))
			return false;
		if (TopWalls.count(*id))
			return false;
	}
	
	//if (SoldiObjectCollision(id, current))
	//{
	//	return false; 
	//}
	return true;
}

vector<GridLocation> Grid::Neighbors(GridLocation* id)
{
	int x, y, dx, dy;
	tie(x, y) = *id;
	vector<GridLocation> results;
	Tile current = GetTileByGridLocation(id);
	for (auto dir : DIRS) 
	{
		tie(dx, dy) = std::get<1>(dir);
		GridLocation next(x + dx, y + dy);
		if (InBounds(next) && IsPassable(std::get<0>(dir), &next, current)) 
		{
			//if(!SoldiObjectCollision(id, current))
				results.push_back(next);
		}
	}
	if (results.size() > 0 && (x + y) % 2 == 0)
		std::reverse(results.begin(), results.end());
	return results;
}

bool Grid::SoldiObjectCollision(GridLocation* id, Tile current)
{
	//Check if the line between current tile and next tile collide with any solid objects 
	SDL_Rect* currentLocation = current.GetLocation();
	SDL_Rect* nextLocation = GetTileByGridLocation(id).GetLocation();
	Line tileToTileLine = Line(currentLocation->x, currentLocation->y, nextLocation->x, nextLocation->y);

	//Door collision 
	auto doors = _DoorManager.GetDoorLocations();
	for(auto door : doors)
	{
		if (SDL_IntersectRectAndLine(door, &tileToTileLine.StartX, &tileToTileLine.StartY,
			&tileToTileLine.EndX, &tileToTileLine.EndY))
			return true;
	}
	return false;
}


double Grid::Heuristic(GridLocation a, GridLocation b)
{
	int x1, y1, x2, y2;
	tie(x1, y1) = a;
	tie(x2, y2) = b;
	return abs(x1 - x2) + abs(y1 - y2);
}


vector<GridLocation> Grid::Search(GridLocation start, GridLocation goal)
{
	PriorityQueue<GridLocation, double> frontier;
	frontier.put(start, 0);

	unordered_map<GridLocation, GridLocation> came_from;
	came_from[start] = start;

	unordered_map<GridLocation, double>cost_so_far;
	cost_so_far[start] = 0;

	while (!frontier.empty()) 
	{
		auto current = frontier.get();

		if (current == goal) 
		{
			break;
		}

		//TODO if there is a straight path to the player lets break

		for (auto next : Neighbors(&current)) 
		{
			double new_cost = cost_so_far[current] + 1;
			
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) 
			{
				cost_so_far[next] = new_cost;
				double priority = new_cost + Heuristic(next, goal);
				frontier.put(next, priority);
				if(next == goal)
					came_from[next] = current;
				else
					came_from[next] = current;
			}
		}
	}
	return ReconstructPath(start, goal, came_from);
}

void Grid::DrawTiles(SDL_Renderer* renderer, Texture* textTiles, SDL_Rect* camera)
{
	for each (auto tile in _Tiles)
	{
		if (tile.IsAlive())
			tile.Draw(renderer, textTiles, camera);
	}

	_DoorManager.Draw(renderer, textTiles, camera);
}

void Grid::SetTiles(vector<Tile> tiles)
{
	_Tiles = tiles; 
	for (size_t i = 0; i < _Tiles.size(); i++)
	{
		if (_Tiles[i].GetWallLocation() == WallLocation::Impassable)
			Impassable.insert(_Tiles[i].GetGridLocation());

		if (_Tiles[i].GetWallLocation() == WallLocation::WallLocation_Left ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_BottomLeft ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_TopLeft)
		{
			LeftWalls.insert(_Tiles[i].GetGridLocation());
		}

		if (_Tiles[i].GetWallLocation() == WallLocation::WallLocation_Right ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_BottomRight ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_TopRight)
		{
			RightWalls.insert(_Tiles[i].GetGridLocation());
		}

		if (_Tiles[i].GetWallLocation() == WallLocation::WallLocation_Top ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_TopRight ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_TopLeft)
		{
			TopWalls.insert(_Tiles[i].GetGridLocation());
		}

		if (_Tiles[i].GetWallLocation() == WallLocation::WallLocation_Bottom ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_BottomLeft ||
			_Tiles[i].GetWallLocation() == WallLocation::WallLocation_BottomRight)
		{
			BottomWalls.insert(_Tiles[i].GetGridLocation());
		}
	}
}

vector<SDL_Rect> Grid::GetWalledTilesOnCamera(SDL_Rect camera)
{
	std::vector<SDL_Rect> solids;
	for each (Tile tile in _Tiles)
	{
		if (SDL_HasIntersection(tile.GetLocation(), &camera))
		{
			if (tile.GetWallLocation() != WallLocation::WallLocation_None)
			{
				std::vector<SDL_Rect> walls = tile.GetWalls();
				for each (SDL_Rect wall in walls)
				{
					solids.push_back(wall);
				}
			}
		}
	}
	return solids; 
}

SDL_Point Grid::GetTileLocationByGridLocation(GridLocation gridLocation)
{
	for (auto tile : _Tiles)
	{
		if (tile.GetGridLocation() == gridLocation)
		{
			return tile.GetTileLocation();
		}
	}

	return SDL_Point{0,0};
}

GridLocation Grid::GetGridLocationByLocation(SDL_Point location)
{
	SDL_Rect rect = SDL_Rect{location.x, location.y, 1,1};
	for (auto tile : _Tiles)
	{
		if (SDL_HasIntersection(tile.GetLocation(), &rect))
		{
			return tile.GetGridLocation();
		}
	}
	return GridLocation{ 0,0 };
}

Tile Grid::GetTileByGridLocation(GridLocation* location)
{
	for (auto tile : _Tiles)
	{
		if (tile.GetGridLocation() == *location)
		{
			return tile;
		}
	}
	return Tile();
}

vector<SDL_Point> Grid::GetPath(SDL_Rect* spawnLocation, SDL_Rect*  playerLocation)
{
	GridLocation start = GetGridLocationByLocation(SDL_Point{ spawnLocation->x, spawnLocation->y });
	GridLocation goal= GetGridLocationByLocation(SDL_Point{ playerLocation->x, playerLocation->y });

	vector<GridLocation> newPath = Search(start, goal);
	vector<SDL_Point> result;

	for (auto loc : newPath)
	{
		result.push_back(GetTileLocationByGridLocation(loc));
	}
	return result; 
}

vector<GridLocation> Grid::ReconstructPath(GridLocation start, GridLocation goal,
	unordered_map<GridLocation, GridLocation>& came_from)
{
	vector<GridLocation> path;
	GridLocation current = goal;
	path.push_back(current);
	while (current != start) {
		current = came_from[current];
		path.push_back(current);
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}

bool Grid::IsStraightPathObstructed(SDL_Point* from, SDL_Point* to)
{
	SDL_Rect toRect = { to->x, to->y, 0,0 };
	SDL_Rect fromRect = { from->x, from->y, 0, 0 };
	return IsStraightPathObstructed(&fromRect, &toRect);
}

bool Grid::IsStraightPathObstructed(SDL_Rect* from, SDL_Point* to)
{
	SDL_Rect toRect = { to->x, to -> y, 0,0 };
	return IsStraightPathObstructed(from, &toRect);
}

bool Grid::IsStraightPathObstructed(SDL_Rect* enemyLocation, SDL_Rect*  playerLocation)
{
	Line playerToEnemyLine = Line(playerLocation->x, playerLocation->y, enemyLocation->x, enemyLocation->y); 
	for each (Tile tile in _Tiles)
	{
		if (tile.GetWallLocation() != WallLocation::WallLocation_None && tile.GetWallLocation() != WallLocation::Impassable)
		{
			std::vector<SDL_Rect> walls = tile.GetWalls();
			for each (SDL_Rect wall in walls)
			{
				if (SDL_IntersectRectAndLine(&wall, &playerToEnemyLine.StartX, &playerToEnemyLine.StartY,
					&playerToEnemyLine.EndX, &playerToEnemyLine.EndY))
					return true; 
			}
		}
	}
	return false; 
}