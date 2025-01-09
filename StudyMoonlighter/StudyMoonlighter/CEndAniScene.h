#pragma once
#include "CScene.h"
class CEndAniScene :public CScene
{
public:
	CEndAniScene();
	virtual ~CEndAniScene() { Release(); }
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
	FRAME m_tFrame;
	bool m_Finish;
};

