#pragma once

#include <unordered_map>
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "Scence.h"
using namespace std;
#define HUD_SIZE 40
#define TILE_SIZE 16

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame* _instance;

	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer;
	LPD3DXSPRITE spriteHandler;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8 di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	// Info of the game
	int screen_width;
	int screen_height;
	// Current scene info
	int mapHeight, mapWidth;

	unordered_map<int, LPSCENE> scenes;
	int current_scene = -1;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, RECT r, int alpha = 255);
	void DrawFixedPosition(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() const { return screen_width; } 
	int GetScreenHeight() const { return screen_height; }
	// Call multiple time in run-time, const
	void setMapSize(int w, int h) { mapWidth = w; mapHeight = h; }
	int getMapWidth() const { return mapWidth; }
	int getMapHeight() const { return mapHeight; }

	static float SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	static CGame* GetInstance();

	~CGame();
};


