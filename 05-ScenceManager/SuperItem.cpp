#include "SuperItem.h"
#include "ColorBlock.h"
#include "Coin.h"
#include "Mario.h"
#define ANI_SUPER_MUSHROOM		301
#define ANI_SUPER_LEAF			302
#define FORM_SMALL_MARIO		0

SuperItem::SuperItem(ItemType type) : Item(type) {
	itemType = ItemType::SUPER_ITEM;
	setObjectState(ObjectState::ITEM_INVISIBLE);
}

void SuperItem::Render() {
	//RenderBoundingBox();
	if (_state == ObjectState::ITEM_VISIBLE) {
		currentAnimation->Render(x, y);
	}
}

void SuperItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt);

	if (_state == ObjectState::ITEM_VISIBLE) {
		vy += ITEM_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		// Collision problems with other than mario obj
		coEvents.clear();
		CalcPotentialCollisions(coObjects, coEvents);
		//DebugOut(L"[coEvents: %d obj\n", coEvents.size());

		if (coEvents.size() == 0) {
			//x += dx;
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<Block*>(e->obj)) {
					x += dx;
					if (e->ny != 0) {
						vy = 0;
					}
				}
			}
		}
	}
}

void SuperItem::setObjectState(ObjectState s, int marioForm, float mario_x)
{
	CGameObject::setObjectState(s);
	switch (s) {
	case ObjectState::ITEM_INVISIBLE: {
		vy = 0;
		break;
	}
	case ObjectState::ITEM_VISIBLE: {
		LPANIMATION ani = (marioForm == FORM_SMALL_MARIO) ?
			CAnimationSets::GetInstance()->Get(5)->Get(ANI_SUPER_MUSHROOM) :
			CAnimationSets::GetInstance()->Get(5)->Get(ANI_SUPER_LEAF);
		setAnimation(ani);
		if (x <= mario_x) {
			nx = -1;
			vx = -SUPER_MUSHROOM_VX;
		}
		else {
			nx = 1;
			vx = SUPER_MUSHROOM_VX;
		}
		vy -= SUPER_MUSHROOM_INIT_VY;
		break;
	}
	}
}