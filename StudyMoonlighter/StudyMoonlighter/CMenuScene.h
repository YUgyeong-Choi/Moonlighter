#pragma once
#include "CScene.h"
class CMenuScene :public CScene
{
public:
	CMenuScene();
	virtual ~CMenuScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	bool m_bIsOpen;
	int m_iOpenTime;
	int m_iOpenSize;
	int m_iWidth;
};

