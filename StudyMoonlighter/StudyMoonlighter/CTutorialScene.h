#pragma once
#include "CScene.h"
#include "CDungeonScene.h"
class CTutorialScene :public CScene
{
public:
	CTutorialScene();
	virtual ~CTutorialScene() { Release(); }
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
	CDungeonScene* m_TutorialDungeon[2];
	int m_iTutorialIndex;
};

