#pragma once
#include "CScene.h"
#include "CVisitorMale.h"
class CAnimationScene :public CScene
{
public:
	CAnimationScene();
	virtual ~CAnimationScene() { Release(); }
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
	FRAME m_tPortalFrame;
	bool m_bPortalFinish;
	CObject* m_pGrandpa;
};

