#pragma once
#include "CObject.h"
class CShopSellAll :public CObject
{
public:
	CShopSellAll();
	virtual ~CShopSellAll() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void SellAll();
	void OnCollision();
	void Key_Input();
private:
	bool m_bActive;
};

