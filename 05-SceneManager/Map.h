#pragma once

#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


class Map
{
	CSprites* sprites = CSprites::GetInstance();
	LPCWSTR mapFilePath;
	LPCWSTR mapPNG;
	int numRows, numCols;
	int numRowRead, numColRead;
	int tileWidth, tileHeight;
	int id;

	int tilemap[300][300];

public:
	bool isWorldMap;
	Map(int id, LPCWSTR filePath_data, int num_Rows, int  num_Cols, int num_row_read, int num_col_read, int map_width = 16, int map_height = 16);
	~Map();
	void Load();
	void LoadMap();
	void Draw();
	int GetMapWidth() { return numCols * tileWidth; }
	int GetMapHeight() { return numRows * tileHeight + 62; }
};


