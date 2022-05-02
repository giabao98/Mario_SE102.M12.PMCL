#pragma once
#include "Block.h"

class Brick : public Block {
public:
	Brick(float width, float height);
	virtual void Render();
};