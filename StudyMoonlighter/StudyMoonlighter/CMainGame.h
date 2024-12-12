#pragma once
#include "Define.h"
class CMainGame
{
public:
	CMainGame();
	~CMainGame() { Release(); }
public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();
private:
	HDC			m_hDC;
};

