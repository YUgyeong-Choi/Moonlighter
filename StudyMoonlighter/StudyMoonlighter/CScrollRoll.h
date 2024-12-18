#pragma once
#include "CMapObj.h"
class CScrollRoll :public CMapObj
{
public:
	CScrollRoll();
	virtual ~CScrollRoll() { Release();}
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
	
	TUTORIALSCROLL Get_ScrollType() { return m_eTutorialScroll; }
	void Set_ScrollType(TUTORIALSCROLL _type) { m_eTutorialScroll = _type; }
private:
	TUTORIALSCROLL m_eTutorialScroll;
};

