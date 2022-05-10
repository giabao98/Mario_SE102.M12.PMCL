#include "MarioTail.h"
#include "Goomba.h"
void MarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TAIL_BBOX_WIDTH / 2;
	top = y - TAIL_BBOX_HEIGHT / 2;
	right = x + TAIL_BBOX_WIDTH;
	bottom = y + TAIL_BBOX_HEIGHT;
}

void MarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (CCollision::GetInstance()->CheckAABB(this, coObjects->at(i)))
		{
			if (dynamic_cast<CGoomba*>(coObjects->at(i)))
				OnCollisionWithGoomba(coObjects->at(i));
		}
	}
}

void MarioTail::Render()
{
	RenderBoundingBox();
}

void MarioTail::OnCollisionWithGoomba(LPGAMEOBJECT& obj)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
	goomba->SetState(GOOMBA_STATE_DIEBYSHELL);
}
