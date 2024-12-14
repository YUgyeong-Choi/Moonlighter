#pragma once
#include "CScene.h"
class CFieldScene :public CScene
{
public:
	CFieldScene();
	virtual ~CFieldScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
};

