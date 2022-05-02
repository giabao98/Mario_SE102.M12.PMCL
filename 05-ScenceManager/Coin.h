#include "Item.h"

class Coin : public Item {
public:
	float destroy = 0;

	Coin(ItemType type);
	~Coin(){}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	ItemType getItemType() { return itemType; }
	void setObjectState(ObjectState s);
};