#pragma once
#include "CScene.h"
//F1  - Ÿ�� �����ֱ� F2 - Ÿ�� �ε� F3 Ÿ�� ����
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

