#include <string>
#include <fstream>
#include "Utils.h"
#include "Game.h"
#include "Textures.h"
#include "Camera.h"

#define MAX_MAP_LINE			1024
#define MAP_INFO				1
#define MAP_SECTION_ROWS		2
#define MAP_SECTION_UNKNOWN		-1

class CMap {
	int column, row = 0;
	int tileSize = 0;
	int tileColumn, tileRow = 0;
	int tiles[200][200] = { -1, -1 };
	int currentRow = 0; // util for parsing from text.
public:
	CMap(wstring path);
	~CMap();

	int getMapWidth() { return column * tileSize; }
	int getMapHeight() { return row * tileSize; }
	void Load(wstring path);
	void Render(CCamera* c);
	void _ParseSection_ROWS(string line);
	void _ParseSection_INFO(string line);
};