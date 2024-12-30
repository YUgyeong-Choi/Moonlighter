#pragma once
#include "CObject.h"
class InventorySlot :public CObject
{
public:
	InventorySlot();
	InventorySlot(int _x, int _y);
	virtual ~InventorySlot() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	ITEM Get_Item() { return item; }
	void Set_Item(ITEM _item) { item = _item; }
	void Item_Init();
	void Set_ItemNum() { item.num++; }
	void Set_ItemType(ITEMTYPE _item);
public:
	int rowIndex;
	int columnIndex;
	ITEM item;
};

