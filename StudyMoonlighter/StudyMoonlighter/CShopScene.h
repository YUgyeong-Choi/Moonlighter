#pragma once
#include "CScene.h"
class CShopScene : public CScene
{
public:
	CShopScene();
	virtual ~CShopScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
	void Offset() override;
	void Set_ShopOpen() { m_bShopOpen = !m_bShopOpen; }
	bool Get_ShopOpen() { return m_bShopOpen; }
private:
	void CheckOffset();
	void OnCollision();
private:
	float offSetShopX;
	float offSetShopY;
	bool m_bShop;
	RECT m_AddUiCheck;
	bool m_Active;
	bool m_bShopOpen;
	int tick;
	vector<ITEM> m_showcaseItem;
	vector<int> m_showcasePrice;
	vector<bool> m_CanNpcSpawn;
};

