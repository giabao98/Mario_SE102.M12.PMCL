#pragma once

#include <iostream>
#include <fstream>
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


class Map
{

	LPCWSTR MapFilePath;
	LPCWSTR MapPNG;
	int Num_Rows, Num_Cols;
	int Tile_Rows, Tile_Cols;
	int Tile_Width, Tile_Height;
	int Id;

	int tilemap[300][300];

public:
	Map(int ID, LPCWSTR FilePath_data, int Num_Rows, int  Num_Cols, int Tile_rows, int Tile_cols, int map_width = 16, int map_height = 16);
	~Map();
	void Load();
	void LoadMapSprite();
	void Draw();
};
