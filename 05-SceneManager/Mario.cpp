#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "ColorBox.h"
#include "Koopas.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx) && state != MARIO_STATE_IDLE) vx = maxVx;

	if (state == MARIO_STATE_IDLE) {
		if (nx > 0 && vx < 0) { vx = 0; ax = 0; }
		else if (nx < 0 && vx > 0) { vx = 0; ax = 0; }
	}

	if (isSlowFalling)
	{
		if (GetTickCount64() - slowFallingTime >= MARIO_SLOWFALLING_TIME)
		{
			isSlowFalling = false;
			SetState(MARIO_STATE_RELEASE_JUMP);
		}
	}
	if (isFalling)
	{
		if (GetTickCount64() - fallingTime >= MARIO_SLOWFALLING_TIME)
		{
			isFalling = false;
			SetState(MARIO_STATE_RELEASE_JUMP);
		}
	}
	if (state == RACOON_STATE_IS_ATTACKED)
	{
		if (GetTickCount64() - effectTime > 600)
		{
			level = MARIO_LEVEL_BIG;
			ay = MARIO_GRAVITY;
			SetState(MARIO_STATE_IDLE);
		}
	}
	if (isFlying)
	{
		if (GetTickCount64() - flyingTime >= 3000)
		{
			isFlying = false;
			if (!isOnPlatform)SetState(MARIO_STATE_RELEASE_JUMP);
		}
	}
	if (abs(ax) == MARIO_ACCEL_RUN_X)
	{
		IncreaseSpeedStack();
	}
	else {
		if (speedStack > 0)
		{
			if (!isFlying) DecreaseSpeedStack();
		}
	}
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (isKickKoopas)
	{
		if (GetTickCount64() - kickKoopasTime >= MARIO_KICK_KOOPAS_TIME)
		{
			isKickKoopas = false;
		}
	}
	isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (isAttack)
	{
		if (nx > 0)
			tail->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2 + TAIL_BBOX_WIDTH / 2, y + 6);
		else
			tail->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2 - TAIL_BBOX_WIDTH / 2, y + 6);
		tail->nx = nx;

		tail->Update(dt, coObjects);
		if (GetTickCount64() - attackTime >= RACOON_ATTACK_TIME)
		{
			isAttack = false;
		}
	}

}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}


void CMario::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<QuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithKoopas(e);
	else if (dynamic_cast<FirePiranhaPlant*>(e->obj))
		OnCollisionWithPlant(e);
	else if (e->obj->isitem)
		OnCollisionWithItem(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->level == NORMAL_GOOMBA)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE) {
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else
		{
			goomba->level = NORMAL_GOOMBA;
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_BIG)
				{
					//level = MARIO_LEVEL_SMALL;
					SetState(RACOON_STATE_IS_ATTACKED);
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}



void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e->obj);

	//Check qbrick
	if (!QBrick->innitItemSuccess && QBrick->GetState() != QUESTION_BRICK_STATE_START_INNIT) {
		if (e->ny > 0)QBrick->SetState(QUESTION_BRICK_STATE_START_INNIT);
	}
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
	if (e->ny < 0)
	{
		if (!isOnPlatform)
		{
			if (koopas->level < PARA_KOOPAS)
			{

				switch (koopas->GetState())
				{
				case KOOPAS_STATE_WALKING:
					koopas->SetState(KOOPAS_STATE_INSHELL);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					break;
				case KOOPAS_STATE_INSHELL:
					koopas->SetState(KOOPAS_STATE_INSHELL_ATTACK);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					break;
				case KOOPAS_STATE_INSHELL_ATTACK:
					koopas->SetState(KOOPAS_STATE_INSHELL);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					break;
				default:
					break;
				}
			}
		}
	}
	else // hit by Koopas
	{
		if (untouchable == 0)
		{
			if (koopas->isAttack)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
			else if (e->nx != 0)
			{
				koopas->nx = nx;
				SetState(MARIO_STATE_KICKKOOPAS);
				koopas->SetState(KOOPAS_STATE_INSHELL_ATTACK);
			}
		}
	}
}

void CMario::OnCollisionWithItem(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<Mushroom*>(e->obj))
	{
		level = MARIO_LEVEL_BIG;
		y -= 16;
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}




//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (state == MARIO_STATE_IDLE)

				{
					if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
				}
				else if (state == MARIO_STATE_WALKING_RIGHT)
				{
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				}
				else if (state == MARIO_STATE_WALKING_LEFT)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0 && state != MARIO_STATE_IDLE)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X || ax == -MARIO_ACCEL_SLOWING_DOWN_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (speedStack == MARIO_MAX_SPEED_STACK)
						aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					else
						aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (ax > 0 && state != MARIO_STATE_IDLE)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X || ax == MARIO_ACCEL_SLOWING_DOWN_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (speedStack == MARIO_MAX_SPEED_STACK)
						aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
					else
						aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				}

			}
	if (isKickKoopas) {
		if (nx > 0)
			aniId = ID_ANI_SMALLMARIO_KICKKOOPAS_RIGHT;
		else
			aniId = ID_ANI_SMALLMARIO_KICKKOOPAS_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRacoon()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (vy >= 0)
			{
				if (nx >= 0)
					aniId = ID_ANI_RACOON_FALLING_RIGHT;
				else
					aniId = ID_ANI_RACOON_FALLING_LEFT;
			}
			else {
				if (nx >= 0)
					aniId = ID_ANI_RACOON_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_RACOON_JUMP_WALK_LEFT;
			}
		}
		if (isFlying)
		{
			if (vy > 0)
			{
				if (nx > 0)aniId = ID_ANI_RACOON_FALLING_FLYING_RIGHT;
				else aniId = ID_ANI_RACOON_FALLING_FLYING_LEFT;
			}
			else if (vy < 0)
			{
				if (nx > 0)aniId = ID_ANI_RACOON_FLYING_RIGHT;
				else aniId = ID_ANI_RACOON_FLYING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_RACOON_SITTING_RIGHT;
			else
				aniId = ID_ANI_RACOON_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (state == MARIO_STATE_IDLE)

				{
					if (nx > 0) aniId = ID_ANI_RACOON_IDLE_RIGHT;
					else aniId = ID_ANI_RACOON_IDLE_LEFT;
				}
				else if (state == MARIO_STATE_WALKING_RIGHT)
				{
					aniId = ID_ANI_RACOON_WALKING_RIGHT;
				}
				else if (state == MARIO_STATE_WALKING_LEFT)
					aniId = ID_ANI_RACOON_WALKING_LEFT;
			}
			else if (vx > 0)
			{
				if (ax == -MARIO_ACCEL_WALK_X || ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_RACOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_RACOON_WALKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (speedStack == MARIO_MAX_SPEED_STACK)
						aniId = ID_ANI_RACOON_RUNNING_RIGHT;
					else
						aniId = ID_ANI_RACOON_WALKING_RIGHT;
				}
				else if (ax == -MARIO_ACCEL_SLOWING_DOWN_X)
					aniId = ID_ANI_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == MARIO_ACCEL_WALK_X || ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_RACOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (speedStack == MARIO_MAX_SPEED_STACK)
						aniId = ID_ANI_RACOON_RUNNING_LEFT;
					else
						aniId = ID_ANI_RACOON_WALKING_LEFT;
				}
				else if (ax == MARIO_ACCEL_SLOWING_DOWN_X)
					aniId = ID_ANI_RACOON_WALKING_LEFT;
			}
	if (isSlowFalling)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_SLOWFALLING_RIGHT;
		else
			aniId = ID_ANI_MARIO_SLOWFALLING_LEFT;
	}
	if (isKickKoopas) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_KICKKOOPAS_RIGHT;
		else
			aniId = ID_ANI_MARIO_KICKKOOPAS_LEFT;
	}
	//if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;
	if (aniId == -1) {
		aniId = ID_ANI_RACOON_IDLE_RIGHT;
	}
	if (isAttack)
	{
		if (level == MARIO_LEVEL_RACOON) {
			if (nx > 0)aniId = ID_ANI_RACOON_ATTACK_RIGHT;
			else aniId = ID_ANI_RACOON_ATTACK_LEFT;
		}
	}
	if (state == RACOON_STATE_IS_ATTACKED)
	{
		aniId = ID_ANI_RACOON_EFFECT_WHEN_ATTACKED;
	}

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (state == MARIO_STATE_IDLE)

				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_IDLE_LEFT;
				}
				else if (state == MARIO_STATE_WALKING_RIGHT)
				{
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
				else if (state == MARIO_STATE_WALKING_LEFT)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}
			else if (vx > 0)
			{
				if (ax == -MARIO_ACCEL_WALK_X || ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (speedStack == MARIO_MAX_SPEED_STACK)
						aniId = ID_ANI_MARIO_RUNNING_RIGHT;
					else
						aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
				else if (ax == -MARIO_ACCEL_SLOWING_DOWN_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax == MARIO_ACCEL_WALK_X || ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (speedStack == MARIO_MAX_SPEED_STACK)
						aniId = ID_ANI_MARIO_RUNNING_LEFT;
					else
						aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
				else if (ax == MARIO_ACCEL_SLOWING_DOWN_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}
	if (isKickKoopas) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_KICKKOOPAS_RIGHT;
		else
			aniId = ID_ANI_MARIO_KICKKOOPAS_LEFT;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (!isAttack && level == MARIO_LEVEL_RACOON)
	{
		animations->Get(ID_ANI_RACOON_ATTACK_LEFT)->ResetAni();
		animations->Get(ID_ANI_RACOON_ATTACK_RIGHT)->ResetAni();
	}

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON)
		aniId = GetAniIdRacoon();

	animations->Get(aniId)->Render(x, y);

	RenderBoundingBox();

	if (isAttack)tail->Render();

	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;
	case MARIO_STATE_RELEASE_JUMP:
		//if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		//if (vy < 0 && !isFlying) vy = 0;
		ay = MARIO_GRAVITY;
		break;
	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f; ax = 0;
			y += MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;
	case MARIO_STATE_IDLE:
		if (vx != 0) {
			ax = -nx * MARIO_ACCEL_SLOWING_DOWN_X; // TODO: To constant - the slowing down speed
		}
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_KICKKOOPAS:
		vx = 0;
		ax = 0;
		kickKoopasTime = GetTickCount64();
		isKickKoopas = true;
		break;
	case MARIO_STATE_ATTACK:
		isAttack = true;
		attackTime = GetTickCount64();
		break;
	case MARIO_STATE_SLOW_FALLING:
		ay = 0;
		vy = MARIO_SLOW_FALLING_SPEED;
		isSlowFalling = true;
		slowFallingTime = GetTickCount64();
		break;
	case MARIO_STATE_FLYING:
		vy = -0.1f;
		ay = 0;
		isFalling = true;
		fallingTime = GetTickCount64();
		if (!isFlying)
		{
			isFlying = true;
			flyingTime = GetTickCount64();
		}
		break;
	case RACOON_STATE_IS_ATTACKED:
		effectTime = GetTickCount64();
		vx = vy = ax = ay = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_RACOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			if (nx > 0)
				left = x - MARIO_BIG_BBOX_WIDTH / 2 + 1;
			else
				left = x - MARIO_BIG_BBOX_WIDTH / 2 + 2;
			right = left + MARIO_BIG_BBOX_WIDTH - 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}
