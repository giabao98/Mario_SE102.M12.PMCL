#include "Pipe.h"

void Pipe::Render()
{
	CSprites* sprite = CSprites::GetInstance();
	int SpriteY = y - height / 2 + SPRITE_TILE_SIZE / 2;
	for (int i = 0; i < height / SPRITE_TILE_SIZE; i++)
	{
		if (i < 1)
			sprite->Get(ID_SPRITE_PIPE_MOUTH)->Draw(x, SpriteY);
		else
			sprite->Get(ID_SPRITE_PIPE_BODY)->Draw(x, SpriteY);
		SpriteY += SPRITE_TILE_SIZE;
	}
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - height / 2;
	r = l + width;
	b = t + height;
}
