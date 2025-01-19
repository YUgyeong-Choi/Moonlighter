#pragma once
#include "Define.h"
#include "CPotionShop.h"
#include "CGambleKey.h"
class CUiManager
{
public:
	CUiManager() :m_eCurUi(UI_END){ ; }
	~CUiManager() {; }
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
	void Set_UiType(UITYPE _type) { m_eCurUi = _type; }
	UITYPE Get_UiType() { return m_eCurUi; }
	CPotionShop* Get_PotionShop() { return m_potionShop; }
	CGambleKey* Get_Gamble() { return m_gamble; }
	void Get_Moenyui() { m_bGetMoney = true; }
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
	void Basic_Ui(HDC hDC);
	void Dungeon_Ui(HDC hDC);
	void Inven_Ui(HDC hDC);
	void Shop_Ui(HDC hDC);
	void Weapon_Ui(HDC hDC);
	void Boss_Ui(HDC hDC);
	void PotionShop_Ui(HDC hDC);
	void GambleUi(HDC hDC);
private:
	static CUiManager* m_pInstance;
	UITYPE m_eCurUi;
	CPotionShop* m_potionShop;
	bool m_bGetMoney;
	FRAME m_tFrame;
	CGambleKey* m_gamble;
};