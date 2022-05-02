#pragma once
#include "Block.h"
#include "Item.h"
#include "GameObject.h"

class QuestionBlock : public Block {
private:
	ItemType itemType;
	Item* item;
public:
	bool isEmpty = false;

	QuestionBlock(ItemType itemType, float w, float h);
	ItemType getItemType();
	Item* getItem();
	void setItem(Item* it);
	void Render();
	void setObjectState(ObjectState s);
};