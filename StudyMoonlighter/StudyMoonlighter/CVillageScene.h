#pragma once
#include "CScene.h"
//F1  - 타일 보여주기 F2 - 타일 로드 F3 타일 저장
class CVillageScene :public CScene
{
public:
	CVillageScene();
	virtual ~CVillageScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void KeyInput();
};

