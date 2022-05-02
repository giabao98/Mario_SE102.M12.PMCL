#pragma once
#include "Game.h"

class CCamera
{
	float width, height;
	float left, top;
	int mapW, mapH;

public:
	CCamera(int mapW, int mapH, float w, float h);
	~CCamera();
	// Set and return the position of the camera
	void SetPosition(float& playerX, float& playerY);
	float GetWidth();
	float GetHeight();
	void GetPostion(float& l, float& t);

	void addx(int x) { left += x; }
	void addy(int y) { top += y; }
};

// Co the truyen vao vy cua Mario vi trong game goc, Mario bay [UP] qua khoi khu 1 [] thi cam moi theo,
// Mario bay [DOWN] thi khong set cung' (mat' hang` gach. trong game)