#pragma once
#include "CMapObj.h"
class CGolemBreakable :public CMapObj
{
public:
	CGolemBreakable();
	virtual ~CGolemBreakable() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	void OnCollision(CObject* _obj);

	bool Get_Breakable() { return m_bIsBreak; }
private:
	int type;
	bool m_bIsBreak;
};

