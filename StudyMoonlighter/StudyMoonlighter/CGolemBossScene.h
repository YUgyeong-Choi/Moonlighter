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
	void Set_bWakeup() { m_bWake = true; }
	bool Get_bBossOffSet() { return m_bBossOffSetTrue; }
private:
	void BossOffset();
private:
	bool m_bWake;
	bool m_bBossOffSetTrue;
	bool m_bScrollOpen;
};

