#pragma once
#include "debug.h"
#include "GameObject.h"

#define MUSHROOM_WIDTH 16
#define MUSHROOM_HEIGHT 16
#define MUSHROOM_GRAVITY    0.0002f

#define MUSHROOM_INNIT_SPEED    0.02f
#define MUSHROOM_SPEED          0.05f

#define MUSHROOOM_STATE_BEING_INNITED   1
#define MUSHROOOM_STATE_INNITED   2

#define ID_ANI_MUSHROOM 80000
class Mushroom :
	public CGameObject
{
protected:
	bool isInnited;
	float startY;
	int itemType;
public:
	Mushroom(float x, float y) :CGameObject(x, y) {
		isInnited = false;
		startY = y;
		itemType = 1;
		isitem = true;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		if (isInnited)
		{
			vy += MUSHROOM_GRAVITY * dt;
		}
		else if (vy != 0)
		{
			if (startY - y > MUSHROOM_HEIGHT + 0.5)
				SetState(MUSHROOOM_STATE_INNITED);
		}
		CCollision::GetInstance()->Process(this, dt, coObjects);

	}
	void OnNoCollision(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	}

	void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
	{
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
		}
		else if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = -vx;
		}
	}
	void Render() {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
		//RenderBoundingBox();
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x - MUSHROOM_WIDTH / 2;
		top = y - MUSHROOM_HEIGHT / 2;
		right = left + MUSHROOM_WIDTH;
		bottom = top + MUSHROOM_HEIGHT;
	}
	void SetState(int state) {
		switch (state) {
		case MUSHROOOM_STATE_BEING_INNITED:
			vy = -MUSHROOM_INNIT_SPEED;
			break;
		case MUSHROOOM_STATE_INNITED:
			vy = 0;
			isInnited = true;
			vx = MUSHROOM_SPEED;
			break;
		default:break;
		}
	}

	int IsBlocking() {
		return 0;
	}
	int IsCollidable() {
		return 1;
	}
};

