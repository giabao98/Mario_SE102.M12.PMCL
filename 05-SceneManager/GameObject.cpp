#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -1;
	isDeleted = false;
	type = 0;
	objType = 0;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b;
	int ObjectType = 0;
	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	Camera::GetInstance()->GetCamPos(cx, cy);
	if (GetType() != 5)
		CGame::GetInstance()->Draw(x - cx, y - cy, bbox, rect.left, rect.top, rect.right, rect.bottom, 1);
	else
		CGame::GetInstance()->Draw(x -8+width/2 - cx, y - cy, bbox, rect.left, rect.top, rect.right, rect.bottom, 1);
}

CGameObject::~CGameObject()
{

}