#include "Hud.h"

Hud* Hud::_instance = nullptr;

Hud* Hud::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Hud();
	return _instance;
}

void Hud::Render()
{
	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	CGame* game = CGame::GetInstance();
	game->DrawFixedPosition(0, game->GetScreenHeight() - HUD_SIZE, bbox, 0, 0, (float)game->GetScreenWidth(), HUD_SIZE, 255);
}
