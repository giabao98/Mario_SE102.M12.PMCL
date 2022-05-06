#include "NavigationBox.h"
#include "debug.h"

void NavigationBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - NAVIGATION_BBOX_WIDTH / 2;
	top = y - NAVIGATION_BBOX_HEIGHT / 2;
	right = left + NAVIGATION_BBOX_WIDTH;
	bottom = top + NAVIGATION_BBOX_HEIGHT;
}

void NavigationBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += NAVIGATION_BOX_GRAVITY * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void NavigationBox::OnNoCollision(DWORD dt)
{
	x += vx + dt;
	y += vy * dt;
}

void NavigationBox::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny < 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
}
