#pragma once
#include "CInvenSlot.h"
#include "CObject.h"
#include "Define.h"
class CInventory :public CObject
{
public:
	CInventory();
	virtual ~CInventory() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
	void KeyInput();
	void Copy_Inven(vector<vector<CInvenSlot*>>& _inventory);
	void InitXY() { rowIndex = 0; columnIndex = 0; }

private:
	vector<vector<CInvenSlot*>> inventory;
	int rowIndex;
	int columnIndex;
	ITEM _pickUpItem;
};

