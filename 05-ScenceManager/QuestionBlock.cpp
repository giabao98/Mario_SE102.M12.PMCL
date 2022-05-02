#pragma once
#include "QuestionBlock.h"
#define ANI_QUESTION_BLOCK_FILLED 602
#define ANI_QUESTION_BLOCK_EMPTY 603

QuestionBlock::QuestionBlock(ItemType itemType, float width, float height) : Block(width, height) {
	blockType = BlockType::QUESTION_BLOCK;
	this->itemType = itemType;
	setObjectState(ObjectState::QUESTION_BLOCK_FILLED);
}

ItemType QuestionBlock::getItemType() {
	return itemType;
}

Item* QuestionBlock::getItem() {
	return item;
}

void QuestionBlock::setItem(Item* it) {
	item = it;
}

void QuestionBlock::Render()
{
	currentAnimation->Render(x, y);
	//RenderBoundingBox();
}

void QuestionBlock::setObjectState(ObjectState s)
{
	CGameObject::setObjectState(s);
	switch (s) {
	case ObjectState::QUESTION_BLOCK_FILLED:
		setAnimation(CAnimationSets::GetInstance()->Get(2)->Get(ANI_QUESTION_BLOCK_FILLED));
		break;
	case ObjectState::QUESTION_BLOCK_EMPTY:
		isEmpty = true;
		setAnimation(CAnimationSets::GetInstance()->Get(2)->Get(ANI_QUESTION_BLOCK_EMPTY));
		break;
	}

}

