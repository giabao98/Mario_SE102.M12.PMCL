#include "Camera.h"

CCamera::CCamera(int mw, int mh, float w, float h) {
	mapW = mw;
	mapH = mh;
	width = w;
	height = h;
	left = top = 0; //TODO size = screen size, top left theo mario
}
CCamera::~CCamera() {}

void CCamera::SetPosition(float &playerX, float &playerY) {
	left = playerX;
	top = playerY;

	float floatedY = height + HUD_SIZE + height / 4;
	if (playerY > floatedY) {
		top = playerY;
	}
	else {
		top = playerY - floatedY / 4;
	}
	CGame* game = CGame::GetInstance();
	left = left - game->GetScreenWidth() / 2;
	// Bound of the map
	if (left < 0) left = 0;
	if (top < 0) top = 0;
	if (left > mapW - game->GetScreenWidth()) left = mapW - game->GetScreenWidth() * 1.0f;
	if (top > floatedY) {
		top = mapH - game->GetScreenHeight() + HUD_SIZE * 1.0f;
	}

	playerX = left; 
	playerY = top;
}

void CCamera::GetPostion(float& l, float& t) {
	l = left;
	t = top;
}

float CCamera::GetHeight() { return height; }
float CCamera::GetWidth() { return width; }



