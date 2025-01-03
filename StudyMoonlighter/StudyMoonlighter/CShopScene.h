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
private:
	float offSetShopX;
	float offSetShopY;
	float offSetHomeX;
	float offSetHomeY;
	bool m_bShop;
};

