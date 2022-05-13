#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.0007f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_JUMPING_SPEED 0.1f
#define GOOMBA_FLYING_SPEED 0.25f
#define GOOMBA_DIEBYSHELL_VX	0.03f
#define GOOMBA_DIEBYSHELL_VY	0.3f

#define GOOMBA_PHASE_WALKING	1
#define GOOMBA_PHASE_JUMPING	2
#define GOOMBA_PHASE_FLYING		3

#define NORMAL_GOOMBA	1
#define PARA_GOOMBA		2

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIEBYSHELL	300

#define ID_ANI_GOOMBA_WALKING 20000
#define ID_ANI_GOOMBA_DIE 21000
#define ID_ANI_GOOMBA_DIEBYSHELL 21002

#define ID_ANI_PARAGOOMBA_WALKING 20001
#define ID_ANI_PARAGOOMBA_JUMPING 20002
#define ID_ANI_PARAGOOMBA_FLYING 20003
#define ID_ANI_PARAGOOMBA_NOWING_WALKING 20004
#define ID_ANI_PARAGOOMBA_DIE	21001
#define ID_ANI_PARAGOOMBA_DIEBYSHELL	21003



class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	bool paraGoomba;

	DWORD phaseTime;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);

	void CalcGoombaMove() {
		switch (goombaPhase) {
			case GOOMBA_PHASE_WALKING:
			{
				if (phaseTime == 0) {
					phaseTime = GetTickCount64();
				}
				else if(GetTickCount64() - phaseTime > 2000) {
					phaseTime = 0;
					goombaPhase = GOOMBA_PHASE_JUMPING;
				}
				break;
			}
			case GOOMBA_PHASE_JUMPING: {
				if (phaseTime == 0) {
					phaseTime = GetTickCount64();
				}
				else if (GetTickCount64() - phaseTime > 1500) {
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

	void GetParaGoombaAni(int& idAni) {
		if (state == GOOMBA_STATE_DIEBYSHELL)idAni = ID_ANI_PARAGOOMBA_DIEBYSHELL;
		else if (level == PARA_GOOMBA)
		{
			if (goombaPhase == GOOMBA_PHASE_WALKING)idAni = ID_ANI_PARAGOOMBA_WALKING;
			else if (goombaPhase == GOOMBA_PHASE_JUMPING)idAni = ID_ANI_PARAGOOMBA_JUMPING;
			else if (goombaPhase == GOOMBA_PHASE_FLYING)idAni = ID_ANI_PARAGOOMBA_FLYING;
		}
		else if(state == GOOMBA_STATE_WALKING)idAni = ID_ANI_PARAGOOMBA_NOWING_WALKING;
		else idAni = ID_ANI_PARAGOOMBA_DIE;
	};
public: 	
	int level, goombaPhase;
	CGoomba(float x, float y, int Level);
	virtual void SetState(int state);
};