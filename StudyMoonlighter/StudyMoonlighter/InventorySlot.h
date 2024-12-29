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
public:
	int rowIndex;
	int columnIndex;
	ITEM item;
};

