#include "Koopas.h"
#include "debug.h"

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!InShell) {
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
	else {
		top = y - KOOPAS_BBOX_HIDDEN/2;
		bottom = top + KOOPAS_BBOX_HIDDEN;
	}
	left = x - KOOPAS_BBOX_WIDTH / 2;
	right = left + KOOPAS_BBOX_WIDTH;
	
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += KOOPAS_GRAVITY * dt;
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)NavBox->SetPosition(x + KOOPAS_BBOX_WIDTH, y);
		else NavBox->SetPosition(x - KOOPAS_BBOX_WIDTH, y);
		NavBox->Update(dt, coObjects);
		float navX, navY;
		NavBox->GetPosition(navX, navY);
		if (navY - y >= 2)vx = -vx;
		
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Koopas::Render()
{
	RenderBoundingBox();
	NavBox->Render();
}

void Koopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopas::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<Koopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

}

Koopas::Koopas(float x, float y, int Level):CGameObject(x,y)
{
	level = Level;
	SetState(KOOPAS_STATE_WALKING);
	NavBox = new NavigationBox(x, y);
}

void Koopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		IsAttack = true;
		InShell = false;
		break;
	case KOOPAS_STATE_INSHELL:
		vx = 0;
		InShell = true;
		IsAttack = false;
		break;
	case KOOPAS_STATE_INSHELL_ATTACK:
		vx = nx*KOOPAS_WALKING_SPEED * 2.5;
		InShell = true;
		IsAttack = true;
		break;
	default:
		break;
	}
}
