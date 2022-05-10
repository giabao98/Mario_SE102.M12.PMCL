#include "Map.h"
#include "Camera.h"
#include"debug.h"

Map::Map(int ID, LPCWSTR FilePath_data, int Map_rows, int  Map_cols, int Num_row_read, int Num_col_read, int Tile_width, int Tile_height)
{
	this->Id = ID;

	this->MapFilePath = FilePath_data;

	this->Num_Rows = Map_rows;
	this->Num_Cols = Map_cols;

	this->num_row_read = Num_row_read;
	this->num_col_read = Num_col_read;

	this->Tile_Width = Tile_width;
	this->Tile_Height = Tile_height;

	LoadMap();
	Load();
}
void Map::Load()
{
	ifstream f;
	f.open(MapFilePath);
	if (f.fail())
	{
		f.close();
		return;
	}
	for (int i = 0; i < Num_Rows; i++)
	{
		for (int j = 0; j < Num_Cols; j++)
			f >> tilemap[i][j];
	}

	f.close();
}

void Map::LoadMap()
{
	CTextures* texture = CTextures::GetInstance();
	LPTEXTURE texMap = texture->Get(Id);
	int id_sprite = 1;
	for (UINT i = 0; i < num_row_read; i++)
	{
		for (UINT j = 0; j < num_col_read; j++)
		{
			int id_Sprite = Id + id_sprite;
			sprites->Add(id_Sprite, Tile_Width * j, Tile_Height * i, Tile_Width * (j + 1), Tile_Height * (i + 1), texMap);
			id_sprite = id_sprite + 1;
		}
	}

}

void Map::Draw()
{
	if (!IsWorldMap)
	{
		int firstcol = (int)Camera::GetInstance()->GetCamPosX() / 16;
		if (firstcol < 0) { firstcol = 0; }
		int lastcol = ((int)Camera::GetInstance()->GetCamPosX()+CGame::GetInstance()->GetBackBufferWidth()) / 16+1;
		for (UINT i = 0; i < Num_Rows; i++)
		{
			for (UINT j = firstcol; j < lastcol; j++)
			{
				float x = Tile_Width * j+8;
				float y = Tile_Height * i+8;
				if (tilemap[i][j])
				sprites->Get(tilemap[i][j] + Id)->Draw(x, y);
			}
		}
	}
	/*else
	{
		for (UINT i = 0; i < Num_Rows; i++)
		{
			for (UINT j = 0; j <Num_Cols; j++)
			{
				float x = Tile_Width * j;
				float y = Tile_Height * i;
				sprites->Get(tilemap[i][j] + Id)->DrawHUD(x, y);
			}
		}
	}*/
	
}                                                                          


Map::~Map()
{
}