#pragma once
#include "GameObject.h"
#include "SuperItem.h"

#define MARIO_WALKING_SPEED		0.15f 
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define ANI_SMALL_MARIO_IDLE_RIGHT		410
#define ANI_SMALL_MARIO_IDLE_LEFT		411
#define ANI_SMALL_MARIO_WALKING_RIGHT	412
#define ANI_SMALL_MARIO_WALKING_LEFT	413
#define ANI_SMALL_MARIO_JUMPING_RIGHT	414
#define ANI_SMALL_MARIO_JUMPING_LEFT	415

#define ANI_SUPER_MARIO_IDLE_RIGHT		400
#define ANI_SUPER_MARIO_IDLE_LEFT		401
#define ANI_SUPER_MARIO_WALKING_RIGHT	402
#define ANI_SUPER_MARIO_WALKING_LEFT	403
#define ANI_SUPER_MARIO_JUMPING_RIGHT	404
#define ANI_SUPER_MARIO_JUMPING_LEFT	405


#define ANI_RACOON_MARIO_IDLE_RIGHT		420
#define ANI_RACOON_MARIO_IDLE_LEFT		421
#define ANI_RACOON_MARIO_WALKING_RIGHT	422
#define ANI_RACOON_MARIO_WALKING_LEFT	423
#define ANI_RACOON_MARIO_JUMPING_RIGHT	426
#define ANI_RACOON_MARIO_JUMPING_LEFT	427

#define ANI_MARIO_DIE				599

#define MARIO_TAIL_ANI_IDLE_RIGHT	1

#define	FORM_SMALL_MARIO	0
#define	FORM_SUPER_MARIO	1
#define	FORM_RACOON_MARIO	2

#define BBW_RACOON_MARIO				22
#define BBH_RACOON_MARIO				28
#define BBW_SUPER_MARIO					15
#define BBH_SUPER_MARIO					27
#define BBW_SMALL_MARIO					13
#define BBH_SMALL_MARIO					15

#define MARIO_UNTOUCHABLE_TIME 5000


class Mario : public CGameObject
{
	static Mario* _instance;

	int form;

	int untouchable;
	DWORD untouchable_start;
	
	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	static Mario* GetInstance();
	unordered_map<string, bool> marioState;

	Mario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void LevelUp();
	void setObjectState(ObjectState state);
	void SetLevel(int l) { form = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start =(long) GetTickCount64(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};