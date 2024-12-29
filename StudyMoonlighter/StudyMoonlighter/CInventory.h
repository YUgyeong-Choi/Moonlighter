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
public:
	vector<vector<InventorySlot*>> inventory;
	int rowIndex;
	int columnIndex;
};

