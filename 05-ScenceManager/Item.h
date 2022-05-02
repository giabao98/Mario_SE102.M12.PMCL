#pragma once
#include "GameObject.h"
#include "Block.h"

enum class ItemType {
	COIN,
	SUPER_ITEM,
};

class Item : public CGameObject {
protected:
	ItemType itemType;
public: 
	Item(ItemType type);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	virtual void Render() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	ItemType getItemType() { return itemType; }
};

