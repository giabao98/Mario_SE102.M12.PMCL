#pragma once
#include <Windows.h>
#include "Utils.h"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
class Camera
{
private:
	float mario_vx, mario_vy=0, mario_Y;
	float cam_x, cam_y, cam_vx, cam_vy =0;
	int autoMove = 0;
	bool isFollowingMario;
public:

	static Camera* __instance;

	static Camera* GetInstance()
	{
		if (__instance == NULL) __instance = new Camera();
		return __instance;
	}
	Camera();



	void SetCamSpeed()
	{
		if (mario_vy < 0 && cam_y >= 0)
			cam_vy = mario_vy;
		else {
			if (cam_y <= 0)
				cam_y = 0;
			cam_vy = mario_vy*0.5;
		}
		if (cam_vy > 0)
		{
			if (cam_y >= 240)
			{
				isFollowingMario = false;
				cam_vy = 0;
				cam_y = 240;
			}
		}
	}
	bool IsInCam(float objX, float objY)
	{
		return !(objX < cam_x || objX - cam_x>SCREEN_WIDTH || objY < cam_y || objY - cam_y > SCREEN_HEIGHT);
	}
	void SetCamPos(float x, float y) {
		cam_x = x;
		cam_y = y;
	};	
	float GetCamPosX()
	{
		return cam_x;
	}
	float GetCamPosY()
	{
		return cam_y;
	}

	void GetCamPos(float& x, float& y) {
		x = cam_x;
		y = cam_y;
	}
	void Update( DWORD dt);
};

