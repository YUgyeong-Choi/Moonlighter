#pragma once
#include "CObject.h"
class CShowCase :public CObject
{
public:
	CShowCase();
	virtual ~CShowCase() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_Item(ITEM _item) { m_sellItem = _item; }
	void Set_InitItem() { m_sellItem.itemId = ITEM_END; }
private:
	void OnCollision();
	void KeyInput();
private:
	bool m_bActive;
	ITEM m_sellItem;
	int m_iItemMove;
	int y;
	int tick;
};

