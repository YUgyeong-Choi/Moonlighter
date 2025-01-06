#pragma once
#include "Define.h"
#include "CInventory.h"
#include "CInventoryShop.h"
class CUiManager
{
public:
	CUiManager() :m_eCurUi(UI_END), m_Inven(nullptr){ ; }
	~CUiManager() {; }
public:
	void Initialize();
	void Update();
	void Render(HDC hDC);
	void Release();
	void Set_UiType(UITYPE _type) { m_eCurUi = _type; }
	void AddItem(ITEMTYPE _item);
	UITYPE Get_UiType() { return m_eCurUi; }
	CInventory* Get_Inven() { return m_Inven; }
	CInventoryShop* Get_InvenShop() { return m_InvenShop; }
	CInvenSlot* Get_Wepon1() { return inventory[0][5]; }
	CInvenSlot* Get_Wepon2() { return inventory[0][6]; }
	CInvenSlot* Get_Potion() { return inventory[2][6]; }
public:
	static CUiManager* GetInstance()
	{
		if (nullptr == m_pInstance) {
			m_pInstance = new CUiManager;
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
	void Dungeon_Ui(HDC hDC);
	void Inven_Ui(HDC hDC);
	void Shop_Ui(HDC hDC);
	void Weapon_Ui(HDC hDC);
	bool FindItem(ITEMTYPE _item);
private:
	static CUiManager* m_pInstance;
	vector<vector<CInvenSlot*>> inventory;
	UITYPE m_eCurUi;
	CInventory* m_Inven;
	CInventoryShop* m_InvenShop;
};