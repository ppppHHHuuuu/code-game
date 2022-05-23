#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager; // duoc khai bao o game.cpp

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) // khoi tao cac bien thanh vien truoc
{
	scaledSize = ms * ts;
}
Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
			mapFile.ignore();
		}
		mapFile.ignore();
	}

	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x= 0; x< sizeX; x++)
		{
			mapFile.get(c);
			if (c == '2')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<CollisionComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders); 
			}
			mapFile.ignore();

		}
		mapFile.ignore();
	}

	mapFile.ignore();



	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}