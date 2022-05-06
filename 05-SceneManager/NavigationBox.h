#pragma once
#define NAVIGATION_BBOX_WIDTH	16
#define NAVIGATION_BBOX_HEIGHT	28

#define NAVIGATION_BOX_GRAVITY	0.0007f


#include "GameObject.h"
class NavigationBox :
    public CGameObject
{

public:
	bool IsOnPlatform;
	NavigationBox(float x, float y) :CGameObject(x, y) {};

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() { RenderBoundingBox(); };

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);

};

