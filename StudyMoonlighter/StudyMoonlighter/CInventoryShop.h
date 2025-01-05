#pragma once
#include "CInvenSlot.h"
#include "CObject.h"
#include "Define.h"
class CInventoryShop
{
public:
	CInventoryShop();
	virtual ~CInventoryShop() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
	void KeyInput();
	void Copy_Inven(vector<vector<CInvenSlot*>>& _inventory);
	void InitXY() { rowIndex = 0; columnIndex = 0; m_IsInven= true;}
	void Set_Showcase();
	void Off_IsPriceTime() { m_IsPriceTime = false; }
	bool Get_IsPriceTime() { return m_IsPriceTime; }
private:
	vector<vector<CInvenSlot*>> inventory;
	vector<vector<CInvenSlot*>> sellSlots;
	vector<CObject*> m_ShowCase;
	int rowIndex;
	int columnIndex;
	ITEM _pickUpItem;
	bool m_IsInven;
	bool m_IsPriceTime;
};

