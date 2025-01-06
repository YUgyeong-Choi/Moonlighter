#pragma once
#include "CMapObj.h"
class CCashRegister :public CMapObj
{
public:
	CCashRegister();
	virtual ~CCashRegister() { Release(); }
public:
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj) override;
	void OnCollision();
private:
	bool m_animActive;
};

