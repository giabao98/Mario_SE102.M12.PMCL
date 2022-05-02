#pragma once
#include "ColorBlock.h"
ColorBlock::ColorBlock(float width, float height) : Block(width, height) {
	blockType = BlockType::COLOR_BLOCK;
}
