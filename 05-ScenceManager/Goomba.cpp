#include "Goomba.h"
CGoomba::CGoomba()
{
	setObjectState(ObjectState::GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (_state == ObjectState::GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (_state == ObjectState::GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->Get(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::setObjectState(ObjectState state)
{
	CGameObject::setObjectState(state);
	switch (state)
	{
	case ObjectState::GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
	case ObjectState::GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}
