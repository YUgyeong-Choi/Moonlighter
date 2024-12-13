#pragma once
#include "Define.h"
class CObject
{
public:
	enum DIRECTION { LEFT, RIGHT, UP, DOWN, DIR_END };
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

