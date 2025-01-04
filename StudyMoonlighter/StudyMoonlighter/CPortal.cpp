#include "pch.h"
#include "CPortal.h"
#include "CScrollManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"

CPortal::CPortal():m_ePortalType(PORTAL_END), m_bEnter(false), m_bCollision(false)
{
}

void CPortal::Initialize()
{
	m_eOBJID = OBJ_PORTAL;

	m_eRender = RENDER_BACKGROUND;
}

int CPortal::Update()
{
	KeyInput();
	__super::Update_Rect();
	return 0;
}

void CPortal::Late_Update()
{
	m_bCollision = false;
}

void CPortal::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (m_bCollision) {
		COLORREF color = RGB(234, 221, 187);

		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		RoundRect(hDC, m_tInfo.fX + iScrollX+20, m_tInfo.fY - 70 + iScrollY, m_tInfo.fX + 170 + iScrollX, m_tInfo.fY - 20 + iScrollY, 30, 30);

		SelectObject(hDC, hOldBrush);
		SelectObject(hDC, hOldPen);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		Image* image(nullptr);
		Graphics graphics(hDC);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/button_J.png");
		graphics.DrawImage(image, (int)m_tInfo.fX + iScrollX +15, (int)m_tInfo.fY - 75 + iScrollY, 0, 0, 64, 64, UnitPixel);

		SetTextColor(hDC, RGB(0, 0, 0));
		TCHAR szEnter[64];
		_stprintf_s(szEnter, _T("들어가기"));
		TextOut(hDC, (int)m_tInfo.fX + iScrollX + 80, (int)m_tInfo.fY - 55 + iScrollY, szEnter, _tcslen(szEnter));
		
		delete image;
	}
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
			m_bEnter = true;
			if (m_bEnter) {
				CSceneManager::GetInstance()->SetScene(SC_GOLEMBOSS);
			}
			break;
		case SHOP:
			if (m_bEnter) {
				CSceneManager::GetInstance()->SetScene(SC_SHOP);
			}
			m_bCollision = true;
			break;
		case DUNGEON:
			if (m_bEnter) {
				CSceneManager::GetInstance()->SetScene(SC_GOLEMDUNGEON);
			}
			m_bCollision = true;
			break;
		default:
			break;
		}
	}
}

void CPortal::KeyInput()
{
	if (CKeyManager::Get_Instance()->Key_Down('J')) {
		m_bEnter = true;
		
	}
}
