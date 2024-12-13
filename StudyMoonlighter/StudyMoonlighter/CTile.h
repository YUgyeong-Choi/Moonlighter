#pragma once
#include "CObject.h"
class CTile: public CObject
{
public:
	CTile();
	virtual ~CTile() { Release(); }
public:
	bool			Get_DrawID() { return m_bIsWalk; }

	void		Set_DrawID() {m_bIsWalk = !m_bIsWalk; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	bool		m_bIsWalk; // 0이면 걸어도 되는 곳 1이면 걸으면 안되는 곳
};

