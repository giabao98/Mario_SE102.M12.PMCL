#include "QuestionBrick.h"

void QuestionBrick::Render()
{
	int aniId = ID_ANI_QUESTION_BRICK_HAVEITEM;
	if (innitItemSuccess) aniId = ID_ANI_QUESTION_BRICK_HAVEITEM;
	else aniId = ID_ANI_QUESTION_BRICK_HAVENOITEM;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (InitCoin)coin->Render();
	//RenderBoundingBox();
}
