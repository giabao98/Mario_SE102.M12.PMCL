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
	//RenderBoundingBox();
	aniId = -1;
	GetAniFirePlant();
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
	if (fireBullet->isActivate)
	{
		fireBullet->Render();
	}
}

void FirePiranhaPlant::GetAniFirePlant()
{
	if (x > enemyX) {
		if (y == minY) {
			if (enemyY < minY)aniId = ID_ANI_FIREPLANT_ATTACK_LEFT_UP;
			else aniId = ID_ANI_FIREPLANT_ATTACK_LEFT_DOWN;
		}
		else if (enemyY < minY)aniId = ID_ANI_FIREPLANT_LEFT_UP;
		else aniId = ID_ANI_FIREPLANT_LEFT_DOWN;
	}
	else {
		if (y == minY) {
			if (enemyY < minY)aniId = ID_ANI_FIREPLANT_ATTACK_RIGHT_UP;
			else aniId = ID_ANI_FIREPLANT_ATTACK_RIGHT_DOWN;
		}
		else if (enemyY < minY)aniId = ID_ANI_FIREPLANT_RIGHT_UP;
		else aniId = ID_ANI_FIREPLANT_RIGHT_DOWN;
	}

	if (aniId == -1) aniId = ID_ANI_FIREPLANT_LEFT_DOWN;
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


