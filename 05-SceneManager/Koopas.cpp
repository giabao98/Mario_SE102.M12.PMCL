#include "Koopas.h"
#include "debug.h"
#include "QuestionBrick.h"
#include "Goomba.h"

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isHolding)
	{
		if (state != KOOPAS_STATE_DIE_BY_SHELL)
			if (!inShell) {
				top = y - KOOPAS_BBOX_HEIGHT / 2;
				bottom = top + KOOPAS_BBOX_HEIGHT;
			}
			else {
				top = y - KOOPAS_BBOX_HIDDEN / 2;
				bottom = top + KOOPAS_BBOX_HIDDEN;
			}
		left = x - KOOPAS_BBOX_WIDTH / 2;
		right = left + KOOPAS_BBOX_WIDTH;
	}
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isHolding)
	{
		vy += ay * dt;
		if (state == KOOPAS_STATE_WALKING && level == SMART_KOOPAS)
		{
			if (vx > 0)navBox->SetPosition(x + KOOPAS_BBOX_WIDTH, y);
			else navBox->SetPosition(x - KOOPAS_BBOX_WIDTH, y);
			navBox->Update(dt, coObjects);
			float navX, navY;
			navBox->GetPosition(navX, navY);
			if (navY - y >= KOOPAS_NAVBOX_DISTANCE)vx = -vx;

		}
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void Koopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	if (level == NORMAL_KOOPAS)GetKoopasAni(aniId);
	else if (level == SMART_KOOPAS)GetRedKoopasAni(aniId);
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
	//navBox->Render();
}

void Koopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopas::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
		nx = -nx;
	}
	if (dynamic_cast<QuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithKoopas(e);
}

void Koopas::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e->obj);

	//Check qbrick
	if (!QBrick->innitItemSuccess && state == KOOPAS_STATE_INSHELL_ATTACK) {
		if (e->nx != 0)QBrick->SetState(QUESTION_BRICK_STATE_START_INNIT);
	}
}

void Koopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (state == KOOPAS_STATE_INSHELL_ATTACK)
	{
		if (e->nx)
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			goomba->nx = nx;
			if(goomba->GetState()!=GOOMBA_STATE_DIEBYSHELL)
			goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
		}
	}
}

void Koopas::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
	if (koopas->state == KOOPAS_STATE_INSHELL_ATTACK) {
		if (e->nx || e->ny)
			SetState(KOOPAS_STATE_DIE_BY_SHELL);
	}
}

void Koopas::GetKoopasAni(int& IdAni)
{
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)IdAni = ID_ANI_KOOPAS_WALKING_RIGHT;
		else IdAni = ID_ANI_KOOPAS_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_INSHELL || state == KOOPAS_STATE_DIE_BY_SHELL)IdAni = ID_ANI_KOOPAS_INSHELL;
	else if (state == KOOPAS_STATE_INSHELL_ATTACK)IdAni = ID_ANI_KOOPAS_INSHELL_ATTACK;

}

void Koopas::GetRedKoopasAni(int& IdAni)
{
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)IdAni = ID_ANI_REDKOOPAS_WALKING_RIGHT;
		else IdAni = ID_ANI_REDKOOPAS_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_INSHELL || state == KOOPAS_STATE_DIE_BY_SHELL)IdAni = ID_ANI_REDKOOPAS_INSHELL;
	else if (state == KOOPAS_STATE_INSHELL_ATTACK)IdAni = ID_ANI_REDKOOPAS_INSHELL_ATTACK;
}

Koopas::Koopas(float x, float y, int Level):CGameObject(x,y)
{
	level = Level;
	SetState(KOOPAS_STATE_WALKING);
	navBox = new NavigationBox(x, y);
	isAttackedByTail = isHolding = false;
	ay = KOOPAS_GRAVITY;
}

void Koopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		isAttack = true;
		inShell = false;
		break;
	case KOOPAS_STATE_INSHELL:
		vx = 0;
		inShell = true;
		isAttack = false;
		break;
	case KOOPAS_STATE_INSHELL_ATTACK:
		vx = nx*KOOPAS_WALKING_SPEED * 4;
		inShell = true;
		isAttack = true;
		break;
	case KOOPAS_STATE_DIE_BY_SHELL:
		vx = nx * GOOMBA_DIEBYSHELL_VX;
		vy = -GOOMBA_DIEBYSHELL_VY;
		inShell = true;
		isAttack = false;
		break;
	case KOOPAS_STATE_ATTACKED_BY_TAIL:
		vx = nx * GOOMBA_DIEBYSHELL_VX;
		vy = -GOOMBA_DIEBYSHELL_VY;
		inShell = true;
		isAttack = false;
		isAttackedByTail = true;
		break;
	default:
		break;
	}
}
