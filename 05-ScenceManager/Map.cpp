#include "Map.h"

CMap::CMap(wstring path)
{
	Load(path);
}

void CMap::Load(wstring path)
{
	ifstream f;
	f.open(path);

	// current resource section flag
	int section = MAP_SECTION_UNKNOWN;
	char str[MAX_MAP_LINE];
	while (f.getline(str, MAX_MAP_LINE)) {
		string line(str);
		if (line[0] == '#') continue;
		if (line == "[INFO]") {
			section = MAP_INFO;
			continue;
		}
		if (line == "[ROWS]") {
			section = MAP_SECTION_ROWS;
			continue;
		}
		if (line[0] == '[') {
			section = MAP_SECTION_UNKNOWN;
			continue;
		}


		switch (section) {
		case MAP_INFO:
			_ParseSection_INFO(line);
			break;
		case MAP_SECTION_ROWS:
			_ParseSection_ROWS(line);
			break;
		}
	}
	f.close();
	DebugOut(L"[MAP] loaded successfully\n");
}

void CMap::Render(CCamera* c)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			int id = tiles[i][j];
			if (id != -1) {
				RECT r;
				r.left = id % tileColumn * tileSize;
				r.top = (id / tileColumn) * tileSize;
				r.right = r.left + tileSize;
				r.bottom = r.top + tileSize;

				/*float c_left, c_top;
				c->GetPostion(c_left, c_top);
				float c_width = c->GetWidth();
				float c_height = c->GetHeight();*/
				/*if (r.left > c_left + c_width || r.right < c_left || r.top > c_top + c_height || r.bottom < c_top) {
					continue;
				}*/
				CGame::GetInstance()->Draw((float)(j * tileSize), (float)(i * tileSize), CTextures::GetInstance()->Get(TEX_MAP_ID), r.left, r.top, r.right, r.bottom);

			}
		}
	}
}

void CMap::_ParseSection_ROWS(string line)
{
	vector<string> tokens = split(line);

	int size = tokens.size();
	if (size < column) return;
	for (int i = 0; i < column; i++) {
		int id = atoi(tokens[i].c_str()) - 1;	// because when using Tiled to export to json file, the id auto increase 1 unit
		tiles[currentRow][i] = id;
	}
	currentRow++;
}

void CMap::_ParseSection_INFO(string line)
{
	vector<string> tokens = split(line);
	int size = tokens.size();

	if (size < 5) return;
	row = atoi(tokens[0].c_str());
	column = atoi(tokens[1].c_str());
	tileRow = atoi(tokens[2].c_str());
	tileColumn = atoi(tokens[3].c_str());
	tileSize = atoi(tokens[4].c_str());
}

CMap::~CMap()
{
}
