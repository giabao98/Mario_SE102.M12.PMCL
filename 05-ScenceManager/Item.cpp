#include "Item.h"
#define BB_W_SUPER_ITEM 16.0f;
#define BB_H_SUPER_ITEM 16.0f;
Item::Item(ItemType type) : CGameObject() {
	setType(ObjectType::ITEM);
	itemType = type;
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BB_W_SUPER_ITEM;
	bottom = top + BB_H_SUPER_ITEM;
}