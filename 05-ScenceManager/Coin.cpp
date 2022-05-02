#include "Coin.h"
#define ANI_SPINNING_COIN 300

Coin::Coin(ItemType type) : Item(type) {
	itemType = ItemType::COIN;
	setObjectState(ObjectState::ITEM_INVISIBLE);
}

void Coin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);
	
	if (_state == ObjectState::ITEM_VISIBLE) {
		vy += ITEM_GRAVITY * dt;
	}
	if (vy > 0 && y > destroy) {
		isAlive = false;
	}
	y += dy;
}

void Coin::Render() {
	if (_state == ObjectState::ITEM_VISIBLE) {
		currentAnimation->Render(x, y);
		RenderBoundingBox();
	}
}

void Coin::setObjectState(ObjectState s)
{
	CGameObject::setObjectState(s);
	switch (s) {
	case ObjectState::ITEM_VISIBLE: {
		destroy = y - GAME_UNIT;
		vy = -0.3f;
		break;
	}
	case ObjectState::ITEM_INVISIBLE: {
		setAnimation(CAnimationSets::GetInstance()->Get(4)->Get(ANI_SPINNING_COIN));
		vy = 0;
		break;
	}
	}

}