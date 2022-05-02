#pragma once
#include "Block.h"

Block::Block(float width, float height) {
	setType(ObjectType::BLOCK);
	SetBoundingBox(width, height);
}
Block::~Block()
{
}

void Block::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + widthBoundingBox;
	b = y + heightBoundingBox;
}