#pragma once
#include "CMapObj.h"
class CTree1 :public CMapObj
{
public:
	CTree1();
	virtual ~CTree1() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
};

