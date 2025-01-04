#pragma once
#include "Define.h"
#include "CObject.h"
#include "CPlayer.h"

class CShopPlayer :public CObject
{
public:
	CShopPlayer();
	virtual ~CShopPlayer() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Set_State(int hp, int money, int maxHp);
	int Get_Money() { return m_iMoney; }
private:
	int m_iMoney;
};

