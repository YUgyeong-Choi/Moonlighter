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
	void Set_Text(const wchar_t* _text) { text = _text; }
	void Set_TextPos(int _x, int _y) { textX = _x; textY = _y; }
private:
	bool m_bIsOpen;
	bool m_bIsFinish;
	int textX;
	int textY;
	const wchar_t* text;
};

