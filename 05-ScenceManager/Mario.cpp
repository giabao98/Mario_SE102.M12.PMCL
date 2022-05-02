#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Portal.h"
#include "Ground.h"
#include "ColorBlock.h"
#include "QuestionBlock.h"
#include "Item.h"
#include "Coin.h"

#define  MarioAni CAnimationSets::GetInstance()->Get(MARIO_ANI)

Mario* Mario::_instance = NULL;
Mario* Mario::GetInstance()
{
	if (_instance == NULL) {
		_instance = new Mario();
	}
	return _instance;
}

Mario::Mario(float x, float y) : CGameObject()
{
	form = FORM_SMALL_MARIO;
	untouchable = 0;
	setObjectState(ObjectState::MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	marioState["onGround"] = false;
	marioState["isFalling"] = true;
}

void Mario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	// Bounding of map
	int mw = 0, mh = 0;
	mw = CGame::GetInstance()->getMapWidth();
	mh = CGame::GetInstance()->getMapHeight();

	if (x < 1) {
		x = 1;
	}
	if (x > mw - TILE_SIZE) {
		x = mw - TILE_SIZE;
	}
	if (y < 0) {
		y = 0;
	}
	if (y > mh) {
		isAlive = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (_state != ObjectState::MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];

			switch (e->obj->getType()) {
			case ObjectType::BLOCK: {
				auto block = dynamic_cast<Block*>(e->obj);
				if (e->ny < 0)
					marioState["onGround"] = true;

				switch (block->getBlockType()) {
				case BlockType::GROUND:
				case BlockType::BRICK: {
					Ground* ground = dynamic_cast<Ground*>(block);
					break;
				}
				case BlockType::COLOR_BLOCK: {
					ColorBlock* colorBlock = dynamic_cast<ColorBlock*>(block);
					if (e->nx != 0) {
						x += dx;
					}
					break;
				}
				case BlockType::QUESTION_BLOCK: {
					QuestionBlock* questionBlock = dynamic_cast<QuestionBlock*>(block);
					if (e->ny > 0) {
						if (questionBlock->getItemType() == ItemType::COIN && !questionBlock->isEmpty) {
							questionBlock->setObjectState(ObjectState::QUESTION_BLOCK_EMPTY);
							Coin* c = dynamic_cast<Coin*>(questionBlock->getItem());
							c->setObjectState(ObjectState::ITEM_VISIBLE);
						}
						if (questionBlock->getItemType() == ItemType::SUPER_ITEM && !questionBlock->isEmpty) {
							questionBlock->setObjectState(ObjectState::QUESTION_BLOCK_EMPTY);
							SuperItem* s = dynamic_cast<SuperItem*>(questionBlock->getItem());
							s->setObjectState(ObjectState::ITEM_VISIBLE, this->form, this->x);
						}
					}
					break;
				}
				}
				break;
			}
			case ObjectType::ITEM: {
				ItemType itemType = dynamic_cast<Item*>(e->obj)->getItemType();

				switch (itemType)
				{
				case ItemType::SUPER_ITEM: {
					e->obj->isAlive = false;
					LevelUp();
					break;
				}
				default:
					break;
				}
			}
			case ObjectType::ENEMY: {
				if (dynamic_cast<CGoomba*>(e->obj)) {// if e->obj is Goomba 
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->getObjectState() != ObjectState::GOOMBA_STATE_DIE)
						{
							goomba->setObjectState(ObjectState::GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->getObjectState() != ObjectState::GOOMBA_STATE_DIE)
							{
								if (form > FORM_SMALL_MARIO)
								{
									form = FORM_SMALL_MARIO;
									StartUntouchable();
								}
								else
									setObjectState(ObjectState::MARIO_STATE_DIE);
							}
						}
					}
				} // if Goomba
				else if (dynamic_cast<CPortal*>(e->obj))
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				}
				break;
			}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void Mario::Render()
{
	currentAnimation->Render(x, y);
	RenderBoundingBox();
}

void Mario::setObjectState(ObjectState state)
{
	CGameObject::setObjectState(state);
	LPANIMATION ani = NULL;
	switch (state) {
	case ObjectState::MARIO_STATE_DIE: {
		ani = MarioAni->Get(ANI_MARIO_DIE);
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
	case ObjectState::MARIO_STATE_JUMP: {
		if (marioState["onGround"]) {
			vy = -MARIO_JUMP_SPEED_Y;
			marioState["onGround"] = false;
		}
		switch (form) {
		case FORM_SMALL_MARIO: {
			ani = (nx > 0) ?
				MarioAni->Get(ANI_SMALL_MARIO_JUMPING_RIGHT) :
				MarioAni->Get(ANI_SMALL_MARIO_JUMPING_LEFT);
			break;
		}
		case FORM_SUPER_MARIO: {
			ani = (nx > 0) ?
				MarioAni->Get(ANI_SUPER_MARIO_JUMPING_RIGHT) :
				MarioAni->Get(ANI_SUPER_MARIO_JUMPING_LEFT);
			break;
		}
		case FORM_RACOON_MARIO: {
			ani = (nx > 0) ?
				MarioAni->Get(ANI_RACOON_MARIO_JUMPING_RIGHT) :
				MarioAni->Get(ANI_RACOON_MARIO_JUMPING_LEFT);
			break;
		}
		}
		break;
	}
	case ObjectState::MARIO_STATE_WALKING_RIGHT: {
		switch (form) {
		case FORM_SMALL_MARIO: {
			ani = MarioAni->Get(ANI_SMALL_MARIO_WALKING_RIGHT);
			break;
		}
		case FORM_SUPER_MARIO: {
			ani = MarioAni->Get(ANI_SUPER_MARIO_WALKING_RIGHT);
			break;
		}
		case FORM_RACOON_MARIO: {
			ani = MarioAni->Get(ANI_RACOON_MARIO_WALKING_RIGHT);
			break;
		}
		}
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	}
	case ObjectState::MARIO_STATE_WALKING_LEFT: {
		switch (form) {
		case FORM_SMALL_MARIO: {
			ani = MarioAni->Get(ANI_SMALL_MARIO_WALKING_LEFT);
			break;
		}
		case FORM_SUPER_MARIO: {
			ani = MarioAni->Get(ANI_SUPER_MARIO_WALKING_LEFT);
			break;
		}
		case FORM_RACOON_MARIO: {
			ani = MarioAni->Get(ANI_RACOON_MARIO_WALKING_LEFT);
			break;
		}
		}
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	}
	case ObjectState::MARIO_STATE_IDLE: {
		vx = 0;
		switch (form) {
		case FORM_SMALL_MARIO: {
			ani = (nx > 0) ?
				MarioAni->Get(ANI_SMALL_MARIO_IDLE_RIGHT) :
				MarioAni->Get(ANI_SMALL_MARIO_IDLE_LEFT);
			break;
		}
		case FORM_SUPER_MARIO: {
			ani = (nx > 0) ?
				MarioAni->Get(ANI_SUPER_MARIO_IDLE_RIGHT) :
				MarioAni->Get(ANI_SUPER_MARIO_IDLE_LEFT);
			break;
		}
		case FORM_RACOON_MARIO: {
			ani = (nx > 0) ?
				MarioAni->Get(ANI_RACOON_MARIO_IDLE_RIGHT) :
				MarioAni->Get(ANI_RACOON_MARIO_IDLE_LEFT);
			break;
		}
		}
		break;
	}
	}
	DebugOut(L"%d\n", state);
	setAnimation(ani);
}

void Mario::LevelUp()
{
	if (form == FORM_SMALL_MARIO) {
		y -= BBH_SUPER_MARIO;
		SetLevel(FORM_SUPER_MARIO);
	}
}

void Mario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	switch (form) {
	case FORM_SUPER_MARIO:
		right = x + BBW_SUPER_MARIO;
		bottom = y + BBH_SUPER_MARIO;
		break;
	case FORM_SMALL_MARIO:
		right = x + BBW_SMALL_MARIO;
		bottom = y + BBH_SMALL_MARIO;
		break;
	case FORM_RACOON_MARIO:
		right = x + BBW_RACOON_MARIO;
		bottom = y + BBH_RACOON_MARIO;
		break;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	setObjectState(ObjectState::MARIO_STATE_IDLE);
	SetLevel(FORM_RACOON_MARIO);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

