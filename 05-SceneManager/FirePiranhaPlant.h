#pragma once
#include "GameObject.h"
#include "FireBullet.h"

#define FIRE_PIRANHAPLANT_UP_HEIGHT 16
#define FIRE_PIRANHAPLANT_ATTACK_ZONE 100

#define FIRE_PIRANHAPLANT_BBOX_WIDTH 16
#define FIRE_PIRANHAPLANT_BBOX_HEIGHT 34

#define MAX_ATTACK_ZONE	150
#define MIN_ATTACK_ZONE	100

#define FIRE_PIRANHAPLANT_STATE_UP	1
#define FIRE_PIRANHAPLANT_STATE_DOWN	2

class FirePiranhaPlant :
    public CGameObject
{
public:
	float ax;
	float ay;

	float minY, maxY;

	bool isAttack;
	FireBullet* fireBullet;
	float enemyX, enemyY;
	DWORD CalcAtkTime;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	
	void CalcAtatckEnemy() {
		if (abs(x - enemyX) <= MAX_ATTACK_ZONE && abs(y - enemyY) <= MAX_ATTACK_ZONE)
		{
			CreateFireBullet();
		}
	}
	void CreateFireBullet()
	{
		float VxBullet, VyBullet,BulletX,BulletY;
		if (abs(x - enemyX) <= MIN_ATTACK_ZONE)
		{
			if (enemyY < y)
				VyBullet = -0.05;
			else
				VyBullet = 0.05;
		}
		else
		{
			if (enemyY < y)
				VyBullet = -0.03;
			else
				VyBullet = 0.03;
		}
		if (enemyX > x)
		{
			VxBullet = 0.05;
			nx = 1;
			BulletX = x + FIRE_PIRANHAPLANT_BBOX_WIDTH / 2;
		}
		else
		{
			VxBullet = -0.05;
			nx = -1;
			BulletX = x - FIRE_PIRANHAPLANT_BBOX_WIDTH / 2;
		}
		BulletY = minY - 8;
		if (!fireBullet->isActivate)
		{
			fireBullet->isActivate = true;
			fireBullet->SetPosition(BulletX, BulletY);
			fireBullet->SetSpeed(VxBullet, VyBullet);
		}
	}
public:
	void GetEnemyPos(float EnemyX, float EnemyY) {
		enemyX = EnemyX;
		enemyY = EnemyY;
	}
	FirePiranhaPlant(float x, float y) :CGameObject(x, y) {
		minY = y- FIRE_PIRANHAPLANT_UP_HEIGHT*2-8;
		maxY = y + FIRE_PIRANHAPLANT_UP_HEIGHT-8;
		SetState(FIRE_PIRANHAPLANT_STATE_UP);
		fireBullet = new FireBullet(x, y);
		enemyX = enemyY = 0;
	};
	virtual void SetState(int state);
};

