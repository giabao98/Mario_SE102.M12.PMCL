#pragma once
#include "GameObject.h"

#define ID_SPRITE_PIPE_MOUTH 131313
#define ID_SPRITE_PIPE_BODY 141414

#define SPRITE_TILE_SIZE	16

class Pipe : public CGameObject
{
protected:
	int width, height, tileSize;
public:
	Pipe(float x, float y, int width, int height) : CGameObject(x, y) {
		width = width;
		height = height;
	}
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; };
	int IsCollidable() { return 1; }
};


