#pragma once
#include "GameObject.h"
class Pipe :
	public CGameObject
{
public:
	int width, weight;
	Pipe(float x, float y, int width, int height) : CGameObject(x, y) {
		width = width;
		weight = height;
	}
	void Render() {  };
	void Update(DWORD dt) {
		x += vx * dt;
		y += vy * dt;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; };
	int IsCollidable() { return 1; }
};


