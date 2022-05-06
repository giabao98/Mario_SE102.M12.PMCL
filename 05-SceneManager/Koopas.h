#pragma once
#include "GameObject.h"
#include "NavigationBox.h"

#define KOOPAS_GRAVITY 0.0007f
#define KOOPAS_WALKING_SPEED 0.01f

#define KOOPAS_STATE_WALKING	1
#define KOOPAS_STATE_INSHELL	2
#define KOOPAS_STATE_INSHELL_ATTACK		3

#define NORMAL_KOOPAS	1
#define PARA_KOOPAS		2

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 28
#define KOOPAS_BBOX_HIDDEN 16


class Koopas : public CGameObject
{
protected:

	bool ParaKoopas;

	bool InShell;
	DWORD phaseTime;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);

public:
	NavigationBox* NavBox;

	int level;
	bool IsAttack;
	Koopas(float x, float y, int Level);
	virtual void SetState(int state);
};