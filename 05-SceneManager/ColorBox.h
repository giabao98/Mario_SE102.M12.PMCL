#pragma once
#include "GameObject.h"
class ColorBox :
	public CGameObject
{
	int Width;
	int Height;
public:
	ColorBox(int W, int H)
	{
		Width = W;
		Height = H;
		objType = 6;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x-Width/2;
		top = y-Height/2;
		right = left + Width;
		bottom = top + Height;
	}
	virtual int GetWidth() { return Width; };
	virtual void Render();
	virtual void DirectBlocking(int& l, int& t, int& r, int& b) { l = 0; t = 1; b = 0; r = 0; }
};

