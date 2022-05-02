#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

#define MARIO_ANI 0
#define BRICK_ANI 1
#define QUESTION_BRICK_ANI	2

/// <summary>
/// Frame is a sprite + a time period that sprite is displayed (frame time)
/// </summary>
class AnimationFrame
{
	LPSPRITE sprite;
	DWORD time;
public:
	AnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};
typedef AnimationFrame* LPANIMATION_FRAME;
class Animation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
public:
	Animation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y, int alpha = 255);
};
typedef Animation* LPANIMATION;
class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations* GetInstance();
};

class CAnimationSet {
private:
	unordered_map<int, LPANIMATION> animation_set;
	static CAnimationSet* __instance;
public:
	static CAnimationSet* GetInstance();
	void Add(int objectId, LPANIMATION ani);
	LPANIMATION Get(int stateId);
};
typedef CAnimationSet* LPANIMATION_SET;
/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets* __instance;

	unordered_map<int, CAnimationSet*> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static CAnimationSets* GetInstance();
};