#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "GameProperties.h"
#include "Utils.h"

enum class ObjectType {
	PLAYER,
	ENEMY,
	BLOCK,
	ITEM
};

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class CGameObject
{
protected:
	ObjectType type;
	ObjectState _state;
public:
	float x, y;
	float dx, dy;	// dx = vx * dt;	dy = vy * dt
	float vx, vy;

	bool isAlive = true;

	int nx;

	DWORD dt;

	LPANIMATION_SET animation_set;

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;

	~CGameObject();


	// =========================================================
	// Static object
	float widthBoundingBox, heightBoundingBox;

	virtual void SetBoundingBox(float w, float h) {
		widthBoundingBox = w;
		heightBoundingBox = h;
	}

	ObjectType getType() { return this->type; }
	void setType(ObjectType t) { type = t; }

	LPANIMATION currentAnimation;
	void setAnimation(LPANIMATION ani) {
		currentAnimation = ani;
	}

	virtual void setObjectState(ObjectState s);
	ObjectState getObjectState();
};

