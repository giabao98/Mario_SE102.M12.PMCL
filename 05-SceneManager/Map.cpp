#include "Map.h"
#include "Camera.h"
#include"debug.h"

Map::Map(int id, LPCWSTR filePath_data, int Map_rows, int  Map_cols, int num_row_read, int num_col_read, int Tile_width, int Tile_height)
{
	this->id = id;

	this->mapFilePath = filePath_data;

	this->numRows = Map_rows;
	this->numCols = Map_cols;

	this->numRowRead = num_row_read;
	this->numColRead = num_col_read;

	this->tileWidth = Tile_width;
	this->tileHeight = Tile_height;

	LoadMap();
	Load();
}
void Map::Load()
{
	ifstream f;
	f.open(mapFilePath);
	if (f.fail())
	{
		f.close();
		return;
	}
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
			f >> tilemap[i][j];
	}

	f.close();
}

void Map::LoadMap()
{
	CTextures* texture = CTextures::GetInstance();
	LPTEXTURE texMap = texture->Get(id);
	int id_sprite = 1;
	for (UINT i = 0; i < numRowRead; i++)
	{
		for (UINT j = 0; j < numColRead; j++)
		{
			int id_Sprite = id + id_sprite;
			sprites->Add(id_Sprite, tileWidth * j, tileHeight * i, tileWidth * (j + 1), tileHeight * (i + 1), texMap);
			id_sprite = id_sprite + 1;
		}
	}

}

void Map::Draw()
{
	if (!isWorldMap)
	{
		int firstcol = (int)Camera::GetInstance()->GetCamPosX() / 16;
		if (firstcol < 0) { firstcol = 0; }
		int lastcol = ((int)Camera::GetInstance()->GetCamPosX()+CGame::GetInstance()->GetBackBufferWidth()) / 16+1;

		int firstrow = (int)Camera::GetInstance()->GetCamPosY() / 16;
		int lastrow = ((int)Camera::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetBackBufferWidth()) / 16 + 1;
		for (UINT i = firstrow; i < lastrow; i++)
		{
			for (UINT j = firstcol; j < lastcol; j++)
			{
				float x = tileWidth * j+8;
				float y = tileHeight * i+8;
				if (tilemap[i][j])
				sprites->Get(tilemap[i][j] + id)->Draw(x, y);
			}
		}
	}
	/*else
	{
		for (UINT i = 0; i < numRows; i++)
		{
			for (UINT j = 0; j <numCols; j++)
			{
				float x = tileWidth * j;
				float y = tileHeight * i;
				sprites->Get(tilemap[i][j] + id)->DrawHUD(x, y);
			}
		}
	}*/
	
}                                                                          


Map::~Map()
{
}