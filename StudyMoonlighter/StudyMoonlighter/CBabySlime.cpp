#include "pch.h"
#include "CBabySlime.h"
#include "CScrollManager.h"

CBabySlime::CBabySlime()
{
}

void CBabySlime::Initialize()
{
	m_eOBJID = OBJ_MONSTER;
	m_MonsterType = BABYSLIME;

	m_tInfo.fCX = 70.f;
	m_tInfo.fCY = 70.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 70.f;
	m_tRenderSizeY = 70.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_iHp = 15;
	m_fSpeed = 1.f;
}

int CBabySlime::Update()
{
	if (m_iHp <= 0) {
		return OBJ_DEAD;
	}

	float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

	fWidth = m_targetObj->Get_Info().fX - m_tInfo.fX;
	fHeight = m_targetObj->Get_Info().fY - m_tInfo.fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	m_fAngle = fRadian * (180.f / PI);

	if (m_targetObj->Get_Info().fY > m_tInfo.fY)
		m_fAngle *= -1.f;

	// degree to radian
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	__super::Update_Rect();
	return 0;
}

void CBabySlime::Late_Update()
{
	Hit();
	__super::Move_Frame();
}

void CBabySlime::Render(HDC hDC)
{
	Image* image(nullptr);
	image = Image::FromFile(L"../MoonlighterAssets/Map/Dungeon1/babyslime/babyslime_walk.png");
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, _T("Slime: Hp=%d"),  m_iHp);
	//_stprintf_s(szBuffer, _T("Player: X=%d, Y=%d"), (int)iScrollX, (int)iScrollY);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 300, 10, szBuffer, _tcslen(szBuffer));

	delete image;
}

void CBabySlime::Release()
{
}

void CBabySlime::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER) {
		if (m_bCanHit) {
			if (m_fAttacktedTime + 1000 < GetTickCount64()) {
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
			}
		}
	}
}
