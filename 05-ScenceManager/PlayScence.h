#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "QuestionBlock.h"
#include "Camera.h"
#include "Hud.h"

class CPlayScene : public CScene
{
protected:

	Mario* mPlayer;					// A play scene has to have player, right? 
	CMap* mMap;

	vector<LPGAMEOBJECT> objects;
public:
	CPlayScene(int id, LPCWSTR filePath);
	float cam_x = 0.0f, cam_y = 0.0f;

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	Mario* GetPlayer() { return mPlayer; }
	//CCamera* GetCamera() { return camera; }
	CCamera* camera;

	vector<QuestionBlock*> questionBlocks;
	vector<Item*> items;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
};

class CPlayScenceKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};

