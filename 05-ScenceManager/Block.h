#pragma once
#include "GameObject.h"

enum class BlockType {
	UNDEFINED = -1,
	COLOR_BLOCK,
	GROUND,
	BRICK,
	QUESTION_BLOCK
};

class Block : public CGameObject
{
protected:
	BlockType blockType = BlockType::UNDEFINED;
public:
	Block(float width, float height);
	~Block();
	virtual void Render() {
		//RenderBoundingBox();
	}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	BlockType getBlockType() { return blockType; }
};
