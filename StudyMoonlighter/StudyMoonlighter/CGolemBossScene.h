#pragma once
#include "CScene.h"
class CGolemBossScene :public CScene
{
public:
	CGolemBossScene();
	virtual ~CGolemBossScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
	void Offset() override;
};

