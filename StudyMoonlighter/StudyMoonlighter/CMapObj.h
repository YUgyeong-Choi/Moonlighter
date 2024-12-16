#pragma once
#include "CObject.h"
class CMapObj :public CObject
{
public:
	CMapObj();
	virtual ~CMapObj() { Release(); }
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

	MAPOBJ Get_MapObjType() { return m_mapType; }
protected:
	MAPOBJ m_mapType;
};

