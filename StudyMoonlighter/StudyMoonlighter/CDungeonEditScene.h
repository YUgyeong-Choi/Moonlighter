#pragma once
#include "CScene.h"
class CDungeonEditScene :public CScene
{
public:
	CDungeonEditScene();
	virtual ~CDungeonEditScene() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void LateUpdate() override;
	void Render(HDC hDC) override;
	void Release() override;
	void Key_Input() override;
	void Create_MapObj() override;
private:
	bool m_bIsShowTile;
};

