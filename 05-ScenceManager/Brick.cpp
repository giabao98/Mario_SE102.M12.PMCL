#include "Brick.h"

Brick::Brick(float width, float height) : Block(width, height) {
	blockType = BlockType::BRICK;
}

void Brick::Render() {
	RenderBoundingBox();
}
