#pragma once
#include "Define.h"
class CPotionShop 
{
public:
	CPotionShop();
	virtual ~CPotionShop() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
	void KeyInput();
	void InitXY() { rowIndex = 0; columnIndex = 0;}
private:
	int rowIndex;
	int columnIndex;
	FRAME m_tFrame;
	bool m_craft;
};

