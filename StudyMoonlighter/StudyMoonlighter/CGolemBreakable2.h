#pragma once
#include "CMapObj.h"
class CGolemBreakable2 :public CMapObj
{
public:
	CGolemBreakable2();
	virtual ~CGolemBreakable2() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);
	void OnCollision();

	bool Get_Breakable() { return m_bIsBreak; }
private:
	int type;
	bool m_bIsBreak;
};

