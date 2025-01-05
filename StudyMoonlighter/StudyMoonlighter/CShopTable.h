#pragma once
#include "CObject.h"
class CShopTable :public CObject
{
public:
	CShopTable();
	virtual ~CShopTable() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void OnCollision();
private:
	bool m_bActive;
};

