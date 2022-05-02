#pragma once
#include "Textures.h"
#include "Game.h"

#define HUD_SIZE 40
#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class Hud
{
private:
	static Hud* _instance;
	float cam_y;
public:
	static Hud* GetInstance();
	void Render();
};

