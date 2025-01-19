#include "pch.h"
#include "CInventoryManager.h"
#include "CSpecialSlot.h"

CInventoryManager* CInventoryManager::m_pInstance = nullptr;

void CInventoryManager::Initialize()
{
	inventory.resize(4);
	for (int i = 0; i < 4; ++i) {
		inventory[i].resize(7);
		for (int j = 0; j < 7; ++j) {
			inventory[i][j] = nullptr;
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			inventory[i][j] = new CInvenSlot(i, j);
		}
	}

	inventory[1][5] = new CSpecialSlot(HELMET);
	inventory[2][5] = new CSpecialSlot(ARMOR);
	inventory[3][5] = new CSpecialSlot(BOOTS);
	inventory[0][5] = new CSpecialSlot(WEAPON1);
	inventory[0][6] = new CSpecialSlot(WEAPON2);
	inventory[2][6] = new CSpecialSlot(OUTFIT_POTION);

	m_Inven = new CInventory();
	m_Inven->Copy_Inven(inventory);

	m_InvenShop = new CInventoryShop();
	m_InvenShop->Initialize();
	m_InvenShop->Copy_Inven(inventory);
}

void CInventoryManager::Update()
{
	m_Inven->Update();
	m_InvenShop->Update();
}

void CInventoryManager::Render(HDC hDC)
{
}

void CInventoryManager::Release()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 7; ++j) {
			Safe_Delete<CInvenSlot*>(inventory[i][j]);
		}
	}
	Safe_Delete<CInventory*>(m_Inven);
	Safe_Delete<CInventoryShop*>(m_InvenShop);
}

void CInventoryManager::AddItem(ITEMTYPE _item)
{
	if (FindItem(_item)) {
		return;
	}
	else {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 5; ++j) {
				if (inventory[i][j]->Get_Item().itemId == ITEM_END) {
					inventory[i][j]->Set_ItemType(_item);
					return;
				}
			}
		}
	}
}

bool CInventoryManager::FindItem(ITEMTYPE _item)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (_item == inventory[i][j]->Get_Item().itemId) {
				if (inventory[i][j]->Get_Item().maxNum > inventory[i][j]->Get_Item().num) {
					inventory[i][j]->Add_ItemNum();
					return true;
				}
			}
		}
	}
	return false;
}
