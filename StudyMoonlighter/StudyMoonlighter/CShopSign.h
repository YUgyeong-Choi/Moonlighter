#pragma once
#include "CObject.h"
class CShopSign :public CObject
{
public:
	CShopSign();
	virtual ~CShopSign() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void OnCollision();
	void Key_Input();
private:
	bool m_bActive;
};

