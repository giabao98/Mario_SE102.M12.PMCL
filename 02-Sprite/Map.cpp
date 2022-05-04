#include "Map.h"
Map::Map(int ID, LPCWSTR FilePath_data, int Map_rows, int  Map_cols, int Tile_rows, int Tile_cols, int Tile_width, int Tile_height)
{
	this->Id = ID;

	this->MapFilePath = FilePath_data;


	// rows of Map
	this->Num_Rows = Map_rows;
	// cols of Map
	this->Num_Cols = Map_cols;

	// rows of tilemap
	this->Tile_Rows = Tile_rows;
	// cols of tilemap
	this->Tile_Cols = Tile_cols;

	this->Tile_Width = Tile_width;
	this->Tile_Height = Tile_height;

	LoadMapSprite();
	Load();
}
void Map::Load()
{
	// Load info map form file
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
		{
			f >> tilemap[i][j];
		}
	}

	f.close();
}

void Map::LoadMapSprite()
{
	// Load sprite for map

	CTextures* texture = CTextures::GetInstance();
	LPTEXTURE texMap = texture->Get(Id);
	int id_sprite = 1;
	for (UINT i = 0; i < Tile_Rows; i++)
	{
		for (UINT j = 0; j < Tile_Cols; j++)
		{
			int id_Sprite = Id + id_sprite;
			CSprites::GetInstance()->Add(id_sprite, Tile_Width * j, Tile_Height * i, Tile_Width * (j + 1), Tile_Height * (i + 1), texMap);
			id_sprite = id_sprite + 1;
		}
	}

}

void Map::Draw()
{
	for (UINT i = 0; i < Num_Rows; i++)
	{
		for (UINT j = 0; j < Num_Cols; j++)
		{
			float x = Tile_Width * j;
			float y = Tile_Height*i;
			CSprites::GetInstance()->Get(tilemap[i][j])->Draw(x, y -240);
		}
	}
}


Map::~Map()
{
}