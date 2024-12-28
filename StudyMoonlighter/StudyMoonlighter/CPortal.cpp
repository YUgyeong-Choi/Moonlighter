#include "pch.h"
#include "CPortal.h"
#include "CScrollManager.h"
#include "CSceneManager.h"

CPortal::CPortal():m_ePortalType(PORTAL_END)
{
}

void CPortal::Initialize()
{
	m_eOBJID = OBJ_PORTAL;

	m_eRender = RENDER_BACKGROUND;
}

int CPortal::Update()
{
	__super::Update_Rect();
	return 0;
}

void CPortal::Late_Update()
{
}

void CPortal::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
	}
}

void CPortal::Release()
{
}

void CPortal::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER) {
		switch (m_ePortalType)
		{
		case VILLAGE:
			CSceneManager::GetInstance()->SetScene(SC_VILLAGE);
			break;
		case FIELD:
			CSceneManager::GetInstance()->SetScene(SC_FIELD);
			break;
		case GOLEMBOSS:
			CSceneManager::GetInstance()->SetScene(SC_GOLEMBOSS);
		case PORTAL_END:
			break;
		default:
			break;
		}
	}
}
