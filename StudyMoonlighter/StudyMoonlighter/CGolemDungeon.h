#pragma once
#include "CScene.h"
#include "CDungeonScene.h"
#include "Define.h"
class CGolemDungeon : public CScene
{
public:
	CGolemDungeon();
	virtual ~CGolemDungeon() { Release(); }
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
	CDungeonScene* m_TutorialDungeon[2][4];
	int m_iTutorialIndeX;
	int m_iTutorialIndeY;
	int m_iMove;
	bool m_bMapMove;
	DIRECTION m_dir;
};

