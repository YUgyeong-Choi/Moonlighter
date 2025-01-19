#pragma once
#include "CInventory.h"
#include "CInventoryShop.h"
class CInventoryManager
{
private:
	CInventoryManager() :m_Inven(nullptr) { ; }
	~CInventoryManager() { Release(); }
public:
	void Initialize();
	void Update();
	void Render(HDC hDC);
	void Release();
	void AddItem(ITEMTYPE _item);
	vector<vector<CInvenSlot*>> Get_Inventory() { return inventory; }
	CInventory* Get_Inven() { return m_Inven; }
	CInventoryShop* Get_InvenShop() { return m_InvenShop; }
	CInvenSlot* Get_Wepon1() { return inventory[0][5]; }
	CInvenSlot* Get_Wepon2() { return inventory[0][6]; }
	CInvenSlot* Get_Potion() { return inventory[2][6]; }
public:
	static CInventoryManager* GetInstance()
	{
		if (nullptr == m_pInstance) {
			m_pInstance = new CInventoryManager;
		}

		return m_pInstance;
	}

	static void DestroyInstance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	bool FindItem(ITEMTYPE _item);
private:
	static CInventoryManager* m_pInstance;
	vector<vector<CInvenSlot*>> inventory;
	CInventory* m_Inven;
	CInventoryShop* m_InvenShop;
	
};

