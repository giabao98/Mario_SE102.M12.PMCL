#include "Goomba.h"
#include "debug.h"

CGoomba::CGoomba(float x, float y, int Level) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	level = Level;
	if (level == PARA_GOOMBA)paraGoomba = true;
	else paraGoomba = false;
	goombaPhase = 1;
	phaseTime = 0;
}

void CGoomba::CalcGoombaMove() {
	switch (goombaPhase) {
	case GOOMBA_PHASE_WALKING:
	{
		if (phaseTime == 0) {
			phaseTime = GetTickCount64();
		}
		else if (GetTickCount64() - phaseTime > 1000) {
			phaseTime = 0;
			goombaPhase = GOOMBA_PHASE_JUMPING;
		}
		break;
	}
	case GOOMBA_PHASE_JUMPING: {
		if (phaseTime == 0) {
			phaseTime = GetTickCount64();
		}
		else if (GetTickCount64() - phaseTime > 1000) {
			phaseTime = 0;
			goombaPhase = GOOMBA_PHASE_FLYING;
		}
		break;
	}
	case GOOMBA_PHASE_FLYING: {
		if (phaseTime == 0) {
			phaseTime = GetTickCount64();
			vy = -GOOMBA_FLYING_SPEED;
		}
		else if (GetTickCount64() - phaseTime > 1000) {
			phaseTime = 0;
			goombaPhase = GOOMBA_PHASE_WALKING;
		}
		break;
	}
	}
}

void CGoomba::GetParaGoombaAni(int& idAni) {
	if (state == GOOMBA_STATE_DIEBYSHELL)idAni = ID_ANI_PARAGOOMBA_DIEBYSHELL;
	else if (level == PARA_GOOMBA)
	{
		if (goombaPhase == GOOMBA_PHASE_WALKING)idAni = ID_ANI_PARAGOOMBA_WALKING;
		else if (goombaPhase == GOOMBA_PHASE_JUMPING)idAni = ID_ANI_PARAGOOMBA_JUMPING;
		else if (goombaPhase == GOOMBA_PHASE_FLYING)idAni = ID_ANI_PARAGOOMBA_FLYING;
	}
	else if (state == GOOMBA_STATE_WALKING)idAni = ID_ANI_PARAGOOMBA_NOWING_WALKING;
	else idAni = ID_ANI_PARAGOOMBA_DIE;
};

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else if (state != GOOMBA_STATE_DIEBYSHELL)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (goombaPhase == GOOMBA_PHASE_JUMPING && level == PARA_GOOMBA)
	{
		if (e->ny < 0) vy = -GOOMBA_JUMPING_SPEED;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	if (level == PARA_GOOMBA)CalcGoombaMove();
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (paraGoomba)GetParaGoombaAni(aniId);
	else {
		if (state == GOOMBA_STATE_DIE)
		{
			aniId = ID_ANI_GOOMBA_DIE;
		}
		else if (state == GOOMBA_STATE_DIEBYSHELL)
			aniId = ID_ANI_GOOMBA_DIEBYSHELL;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBA_STATE_DIEBYSHELL:
		die_start = GetTickCount64();
		vx = nx * GOOMBA_DIEBYSHELL_VX;
		vy = -GOOMBA_DIEBYSHELL_VY;
		break;
	case GOOMBA_STATE_WALKING:
		vx = GOOMBA_WALKING_SPEED;
		break;
	}
}
