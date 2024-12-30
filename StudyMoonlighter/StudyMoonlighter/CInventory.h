#pragma once
#include "CObject.h"
#include "InventorySlot.h"
class CInventory : public CObject
{
public:
	CInventory();
	virtual ~CInventory() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput();

	void AddItem(ITEMTYPE _item);
	void InitXY() { rowIndex = 0; columnIndex = 0; }
private:
	bool FindItem(ITEMTYPE _item);
public:
	vector<vector<InventorySlot*>> inventory;
	int rowIndex;
	int columnIndex;
	ITEM _pickUpItem;
};

