#pragma once
#include "Define.h"
class CScrollManager
{
private:
	CScrollManager();
	~CScrollManager();

public:
	float			Get_ScrollX() { return m_fScrollX; }
	float			Get_ScrollY() { return m_fScrollY; }

	void			Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void			Set_ScrollY(float _fY) { m_fScrollY += _fY; }

	void			Scroll_Lock();
	void			Set_ScrollLock(float _x, float _y) { m_fMapXSize = _x, m_fMapYSize = _y; }

public:
	static CScrollManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollManager;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CScrollManager* m_pInstance;

	float			    m_fScrollX;
	float			    m_fScrollY;

	float m_fMapXSize;
	float m_fMapYSize;
};