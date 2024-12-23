#pragma once
#include "CMapObj.h"
class CGolemScroll : public CMapObj
{
public:
	CGolemScroll();
	virtual ~CGolemScroll() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	bool m_bIsOpen;
	bool m_bIsFinish;
};

