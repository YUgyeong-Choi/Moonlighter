#pragma once
#include "Define.h"
#include "CInventory.h"

class CUiManager
{
public:
	CUiManager():m_eCurUi(UI_END){ m_Inven = new CInventory; }
	~CUiManager() { ; }
public:
	void Update();
	void Render(HDC hDC);
	void Set_UiType(UITYPE _type) { m_eCurUi = _type; }
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
	void Village_Ui(HDC hDC);
	void Inven_Ui(HDC hDC);

private:
	static CUiManager* m_pInstance;
	CInventory* m_Inven;
	UITYPE m_eCurUi;
};