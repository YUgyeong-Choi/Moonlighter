#pragma once
#include "Define.h"
class CObject
{
public:
	CObject();
	virtual ~CObject() { Release(); }
public:
	virtual void		Initialize()PURE;
	virtual int			Update()PURE;
	virtual void		Late_Update()PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release()PURE;

	virtual void		OnCollision(CObject* _obj);
public:
	const INFO* Get_Info_Pointer() { return &m_tInfo; }
	const RENDERID Get_GroupID() const { return m_eRender; }
	OBJID Get_OBJID() { return m_eOBJID; }
	float Get_Speed() { return m_fFixScrollSpeed; }
	RECT* Get_Rect() { return &m_tRect; }
	INFO Get_Info() { return m_tInfo; }
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x, m_tInfo.fY = _y, m_originX = _x, m_originY = _y; }
	void Set_Size(float _x, float _y) { m_tInfo.fCX = _x, m_tInfo.fCY = _y; }
public:
	void		Update_Rect();
	void		Move_Frame();
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	OBJID		m_eOBJID;

	float		m_fSpeed;
	float m_fFixScrollSpeed;

	//원래 좌표
	float m_originX;
	float m_originY;

	//애니메이션 관련
	FRAME		m_tFrame;
	const TCHAR* m_pImgKey;

	RENDERID	m_eRender;
	float		m_tRenderSizeX;
	float		m_tRenderSizeY;
	RECT		m_tRenderRect;
};

