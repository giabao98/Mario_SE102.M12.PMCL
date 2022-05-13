#pragma once
#include "GameObject.h"
class ColorBox :
	public CGameObject
{
	int width;
	int height;
public:
	ColorBox(int w, int h)
	{
		width = w;
		height = h;
		objType = 6;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x - width / 2;
		top = y - height / 2;
		right = left + width;
		bottom = top + height;
	}
	virtual int GetWidth() { return width; };
	virtual void Render();
	virtual void DirectBlocking(int& l, int& t, int& r, int& b) { l = 0; t = 1; b = 0; r = 0; }
};

