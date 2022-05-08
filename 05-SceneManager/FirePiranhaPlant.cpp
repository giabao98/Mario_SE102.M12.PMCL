#include "FirePiranhaPlant.h"
#include "debug.h"

void FirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIRE_PIRANHAPLANT_BBOX_WIDTH / 2;
	top = y - FIRE_PIRANHAPLANT_UP_HEIGHT / 2;
	right = left + FIRE_PIRANHAPLANT_BBOX_WIDTH;
	bottom = top + FIRE_PIRANHAPLANT_BBOX_HEIGHT;
}

void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (vy < 0) {
		if (y <= minY) {
			vy = 0;
			y = minY;
		}
	}
	else if (vy > 0) {
		if (y >= maxY) {
			vy = 0;
			y = maxY;
		}
	}
	if (y == minY) {
		CalcAtatckEnemy();
	}
	if (state == FIRE_PIRANHAPLANT_STATE_UP) {
		if (GetTickCount64() - CalcAtkTime >= 3000) {
			SetState(FIRE_PIRANHAPLANT_STATE_DOWN);
		}
	}
	else {
		if (GetTickCount64() - CalcAtkTime >= 2000) {
			SetState(FIRE_PIRANHAPLANT_STATE_UP);
		}
	}

	if (fireBullet->isActivate)
		fireBullet->Update(dt,coObjects);
}

void FirePiranhaPlant::Render()
{
	RenderBoundingBox();
	if (fireBullet->isActivate)
	{
		fireBullet->Render();
	}
}

void FirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_PIRANHAPLANT_STATE_UP:
		vy = -0.05;
		CalcAtkTime = GetTickCount64();
		break;
	case FIRE_PIRANHAPLANT_STATE_DOWN:
		vy = 0.05;
		CalcAtkTime = GetTickCount64();
		break;
	default:
		break;
	}
}


