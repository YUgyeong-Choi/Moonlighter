#pragma once
#include "CScene.h"
#include "CObject.h"

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
	void Key_Input() override;
	void Create_MapObj() override;
	void Offset() override;
};

