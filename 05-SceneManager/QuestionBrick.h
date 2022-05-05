#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mushroom.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_UP 7

#define QUESTION_BRICK_STATE_START_INNIT 2
#define QUESTION_BRICK_STATE_INNITED 3


#define QUESTION_BRICK_VY	0.025f
class QuestionBrick : public CGameObject {
public:
	int startY;
	bool readyInnitItem;
	bool innitItemSuccess;
	int Item;

	QuestionBrick(float x, float y, int item ) : CGameObject(x, y) {
		startY = y;
		readyInnitItem = innitItemSuccess = false;
		Item = item;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {

		if (startY - y >= QUESTION_BRICK_UP)
			vy = -vy;
		if (vy > 0 && y >= startY) {
			SetState(QUESTION_BRICK_STATE_INNITED);
		}

		if (!readyInnitItem)y += vy * dt;

	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - BRICK_BBOX_WIDTH / 2;
		t = y - BRICK_BBOX_HEIGHT / 2;
		r = l + BRICK_BBOX_WIDTH;
		b = t + BRICK_BBOX_HEIGHT;

	};
	
	void SetState(int state) {
		switch (state) {
		case QUESTION_BRICK_STATE_START_INNIT:
			vy = -QUESTION_BRICK_VY;
			break;
		case QUESTION_BRICK_STATE_INNITED:
			vy = 0;
			readyInnitItem = true;
			break;
		default:break;
		}
	}
};