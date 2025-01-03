#pragma once
#include "Define.h"
class CInvenSlot
{
public:
	CInvenSlot();
	CInvenSlot(int _row, int _column);
	virtual ~CInvenSlot() { Release(); }
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();

	ITEM Get_Item() { return item; }
	void Set_Item(ITEM _item) { item = _item; }
	void Item_Init();
	void Set_ItemNum() { item.num++; }
	void Set_ItemType(ITEMTYPE _item);
protected:
	int rowIndex;
	int columnIndex;
	ITEM item;
	OUTFITTYPE m_OutfitType;
};

