#pragma once
#include "Define.h"
class CObject
{
public:
	enum DIRECTION { LEFT, RIGHT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, DIR_END };
public:
	CObject();
	virtual ~CObject() { Release(); }
public:
	virtual void		Initialize()PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;
public:
	const INFO* Get_Info_Pointer() { return &m_tInfo; }
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x, m_tInfo.fY = _y; }
	void Set_Size(float _x, float _y) { m_tInfo.fCX = _x, m_tInfo.fCY = _y; }
public:
	void		Update_Rect();
	void		Move_Frame();
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	float		m_fSpeed;
	
	//애니메이션 관련
	FRAME		m_tFrame;
	const TCHAR* m_pImgKey;

};

