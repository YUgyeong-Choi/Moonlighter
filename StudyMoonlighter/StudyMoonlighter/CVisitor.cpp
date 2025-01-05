#include "pch.h"
#include "CVisitor.h"
#include "CScrollManager.h"

CVisitor::CVisitor():m_eCurPattern(CVisitor::END), m_ePrePattern(CVisitor::END), m_fSellX(0), m_fSellY(0), m_fCounterX(0), m_fCounterY(0), m_bSellXY(false), tick(0)
{
}

void CVisitor::Initialize()
{
	m_eOBJID = OBJ_NPC;

	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 80.f;
	m_tRenderSizeY = 80.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_fSpeed = 1.f;
	m_eCurPattern = CVisitor::WALK;

	m_eDir = UP;
}

int CVisitor::Update()
{
	if (m_bSellXY) {
		tick++;
	}
	Move_Npc();
	Change_Motion();
	__super::Update_Rect();
    return 0;
}

void CVisitor::Late_Update()
{
	__super::Move_Frame();
}

void CVisitor::Render(HDC hDC)
{

}

void CVisitor::Release()
{
}

void CVisitor::Change_Motion()
{
	if (m_ePrePattern != m_eCurPattern) {
		switch (m_eCurPattern)
		{
		case CVisitor::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CVisitor::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CVisitor::END:
			break;
		default:
			break;
		}
		m_ePrePattern = m_eCurPattern;
	}
}

void CVisitor::OnCollision(CObject* _obj)
{
	if (!m_bSellXY) {
		m_bSellXY = true;
		m_eDir = LEFT;
		m_eCurPattern = CVisitor::IDLE;
	}
}

void CVisitor::Set_Init(float _x, float _y, ITEM _item, int _price)
{
	m_fSellX = _x; 
	m_fSellY = _y; 
	m_item = _item;
	price = _price;

}

void CVisitor::Move_Npc()
{
	if (!m_bSellXY) {
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

		fWidth = m_fSellX - m_tInfo.fX;
		fHeight = m_fSellY - m_tInfo.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);

		m_fAngle = fRadian * (180.f / PI);

		if (m_fSellY > m_tInfo.fY)
			m_fAngle *= -1.f;

		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


		float x = m_fSellX - m_tInfo.fX;
		float y = m_fSellY - m_tInfo.fY;


		if (fabsf(x) < fabsf(y)) {
			if (y < 0) {
				m_eDir = UP;
			}
			else {
				m_eDir = DOWN;
			}
		}
		else {
			if (x < 0) {
				m_eDir = LEFT;
			}
			else {
				m_eDir = RIGHT;

			}
		}
	}

}

void CVisitor::Render_Talk(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Graphics graphics(hDC);
	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Ui/visitor_talk_base.png");
	graphics.DrawImage(image, (int)m_tInfo.fX + 10+ iScrollX, (int)m_tInfo.fY - 60+ iScrollY, 0, 0, 60, 60, UnitPixel);

	if (100 <= tick && tick < 200) {
		image = Image::FromFile(L"../MoonlighterAssets/Ui/visitortalk_thinkcycle_1.png");
	}
	else if (200 <= tick && tick < 300) {
		image = Image::FromFile(L"../MoonlighterAssets/Ui/visitortalk_thinkcycle_2.png");
	}
	else if (300 <= tick) {
		image = Image::FromFile(L"../MoonlighterAssets/Ui/visitortalk_thinkcycle_3.png");
	}
	graphics.DrawImage(image, (int)m_tInfo.fX + 20 + iScrollX, (int)m_tInfo.fY - 40 + iScrollY, 0, 0, 40, 16, UnitPixel);

	delete image;
}
