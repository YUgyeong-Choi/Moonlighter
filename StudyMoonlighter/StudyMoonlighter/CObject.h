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
	const RENDERID Get_GroupID() const { return m_eRender; }
	float Get_Speed() { return m_fFixScrollSpeed; }
	RECT* Get_Rect() { return &m_tRect; }
	INFO Get_Info() { return m_tInfo; }
	void Set_Pos(float _x, float _y) { m_tInfo.fX = _x, m_tInfo.fY = _y; }
	void Set_Size(float _x, float _y) { m_tInfo.fCX = _x, m_tInfo.fCY = _y; }
public:
	void		Update_Rect();
	void		Move_Frame();
	void		OnCollisionEnter(CObject* _obj);
	void		OnCollisionStay(CObject* _obj);
	void		OnCollisionExit(CObject* _obj);
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	OBJID		m_eOBJID;

	float		m_fSpeed;
	float m_fFixScrollSpeed;


	//애니메이션 관련
	FRAME		m_tFrame;
	const TCHAR* m_pImgKey;

	RENDERID	m_eRender;
	float		m_tRenderSizeX;
	float		m_tRenderSizeY;
	RECT		m_tRenderRect;
};

