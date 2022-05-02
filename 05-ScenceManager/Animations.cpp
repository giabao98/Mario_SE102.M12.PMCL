#include "Animations.h"
#include "Utils.h"

CAnimationSets* CAnimationSets::__instance = NULL;
CAnimationSet* CAnimationSet::__instance = NULL;
//============== ANIMATION ============== 
void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new AnimationFrame(sprite, t);
	frames.push_back(frame);
}
// NOTE: sometimes Animation object is NULL ??? HOW ??? 
void Animation::Render(float x, float y, int alpha)
{
	DWORD now = (long) GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}
//============== ANIMATIONS ============== 
CAnimations* CAnimations::__instance = NULL;
CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}
void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}
LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}
void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}
//============== ANIMATION_SETS ============== 
CAnimationSets::CAnimationSets()
{

}
CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}
LPANIMATION_SET CAnimationSets::Get(unsigned int ani_set_id)
{
	LPANIMATION_SET ani_set = animation_sets[ani_set_id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", ani_set_id);

	return ani_set;
}
void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
//============== ANIMATION_SET ============== 
CAnimationSet* CAnimationSet::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSet();
	return __instance;
}
void CAnimationSet::Add(int objectId, LPANIMATION ani) {
	animation_set[objectId] = ani;
}
LPANIMATION CAnimationSet::Get(int stateId) {
	return animation_set[stateId];
}
