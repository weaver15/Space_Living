#pragma once
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <SDL.h>
#include <functional>
#include "Enums.cpp"
#include "Tile.h"
#include "Door.h"

using std::unordered_map;
using std::unordered_set;
using std::array;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;
using std::tuple;
using std::tie;
using std::string;

namespace std 
{
	template <>
	struct hash<tuple<int, int> > {
		inline size_t operator()(const tuple<int, int>& location) const {
			int x, y;
			tie(x, y) = location;
			return x * 1812433253 + y;
		}
	};
}

template<typename T, typename priority_t>
struct PriorityQueue
{
	typedef pair<priority_t, T> PQElement;
	priority_queue<PQElement, vector<PQElement>,
		std::greater<PQElement>> elements;

	inline int size() const { return elements.size(); }

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	inline T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

struct Line
{
	Line();

	Line(int x1, int y1, int x2, int y2)
	{
		StartX = x1;
		StartY = y1;
		EndX = x2;
		EndY = y2;
	};

	int StartX, StartY, EndX, EndY;
};

class Grid
{
public:
	Grid();
	Grid(int width, int);
	~Grid();

	vector<GridLocation> Search(GridLocation start, GridLocation goal);
	void DrawTiles(SDL_Renderer* renderer, Texture* textTiles, SDL_Rect* camera);
	void SetTiles(vector<Tile> tiles);
	vector<SDL_Rect> GetWalledTilesOnCamera(SDL_Rect camera);
	SDL_Point GetTileLocationByGridLocation(GridLocation gridLocation);
	GridLocation GetGridLocationByLocation(SDL_Point location);
	vector<SDL_Point> GetPath(SDL_Rect* enemyLocation, SDL_Rect*  playerLocation);
	bool IsStraightPathObstructed(SDL_Point* from, SDL_Point* to);
	bool IsStraightPathObstructed(SDL_Rect* enemyLocation, SDL_Rect*  playerLocation);
	bool IsStraightPathObstructed(SDL_Rect* from, SDL_Point* to);
	DoorManager _DoorManager;

private:

	int Width, Height;
	unordered_set<GridLocation> LeftWalls, RightWalls, TopWalls, BottomWalls, Impassable;
	array<tuple<Direction, GridLocation>, 4> DIRS;
	vector<Tile> _Tiles;
	bool InBounds(GridLocation id);
	bool IsPassable(Direction dir, GridLocation* id, Tile current);
	vector<GridLocation> Neighbors(GridLocation* id);
	Tile GetTileByGridLocation(GridLocation* gridLocation);
	double Grid::Heuristic(GridLocation a, GridLocation b);
	vector<GridLocation> ReconstructPath(GridLocation start, GridLocation goal, unordered_map<GridLocation, GridLocation>& came_from);
	bool SoldiObjectCollision(GridLocation* id, Tile current);
};

