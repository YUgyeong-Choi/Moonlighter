#pragma once
#include "CMapObj.h"
class CGolemBossRock :public CMapObj
{
public:
	CGolemBossRock();
	virtual ~CGolemBossRock() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);

private:
	int type;
	bool m_bIsBreak;
	float m_tRemoveTime;
};

