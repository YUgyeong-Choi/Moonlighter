#include "pch.h"
#include "CPlayer.h"
#include "CBitManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CTileManager.h"

CPlayer::CPlayer():m_bIsRoll(false), m_eCurState(STATE_END), m_ePreState(STATE_END), m_ePreDir(DIR_END), m_eCurDir(DIR_END), m_fRollTime(0)
{
}

void CPlayer::Initialize()
{
	Load_Assets();
	m_eOBJID = OBJ_PLAYER;

	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 57.f;

	m_fSpeed = 3.f;
	m_fFixScrollSpeed = 3.f;
	m_fRollTime = 25.f;

	//그래픽 관련
	m_pImgKey = L"Will_Idle_down";
	m_eCurState = IDLE;
	m_ePreState = IDLE;
	m_ePreDir = DOWN;
	m_eCurDir = DOWN;
	
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 9;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 80.f;
	m_tRenderSizeY = 80.f;
	m_eRender = RENDER_GAMEOBJECT;
}

int CPlayer::Update()
{
	Key_Input();
	Change_Motion();
	__super::Update_Rect();
	return 0;
}

void CPlayer::Late_Update()
{
	Rolling();
	Offset();
	__super::Move_Frame();
}

void CPlayer::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	HDC		hMemDC = CBitManager::GetInstance()->FindImage(m_pImgKey);
	GdiTransparentBlt(hDC, m_tRenderRect.left + iScrollX, m_tRenderRect.top + iScrollY, m_tRenderSizeX, m_tRenderSizeY, hMemDC, m_tRenderSizeX *m_tFrame.iFrameStart, 0, m_tRenderSizeX, m_tRenderSizeY, RGB(255, 255, 255));
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, _T("Player: X=%d, Y=%d"), (int)m_tInfo.fX - iScrollX, (int)m_tInfo.fY - iScrollY);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 10, 10, szBuffer, _tcslen(szBuffer));

}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT) && CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bIsRoll)
	{
		m_eCurState = IDLE;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP) && CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN) && !m_bIsRoll) {
		m_eCurState = IDLE;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing(VK_UP) && !m_bIsRoll)
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT)) {

			float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX += diagonalSpeed; 
			m_tInfo.fY -= diagonalSpeed; 
			m_eCurDir = UP_RIGHT;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT)) {
			float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX -= diagonalSpeed;
			m_tInfo.fY -= diagonalSpeed;
			m_eCurDir = UP_LEFT;
		}
		else {
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fY -= m_fSpeed;
			m_eCurDir = UP;
		}
		m_eCurState = WALK;

	}else if (CKeyManager::Get_Instance()->Key_Pressing(VK_DOWN) && !m_bIsRoll)
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT)) {
			float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX += diagonalSpeed;
			m_tInfo.fY += diagonalSpeed;
			m_eCurDir = DOWN_RIGHT;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT)) {
			float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX -= diagonalSpeed;
			m_tInfo.fY += diagonalSpeed;
			m_eCurDir = DOWN_LEFT;
		}
		else {
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fY += m_fSpeed;
			m_eCurDir = DOWN;
		}
		m_eCurState = WALK;


	}else if (CKeyManager::Get_Instance()->Key_Pressing(VK_LEFT) && !m_bIsRoll)
	{
		m_fFixScrollSpeed = m_fSpeed;
		m_tInfo.fX -= m_fSpeed;
		m_eCurDir = LEFT;
		m_eCurState = WALK;

	}else if (CKeyManager::Get_Instance()->Key_Pressing(VK_RIGHT) && !m_bIsRoll)
	{
		m_fFixScrollSpeed = m_fSpeed;
		m_tInfo.fX += m_fSpeed;
		m_eCurDir = RIGHT;
		m_eCurState = WALK;
	}else {
		if (!m_bIsRoll) { 
			m_eCurState = IDLE;
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE)) {
		if (!m_bIsRoll) {
			m_bIsRoll = true;
			m_eCurState = ROLL;
			m_fSpeed = 5.f;
		}
	}
}

void CPlayer::Rolling()
{
	if (m_bIsRoll) {
		float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
		switch (m_eCurDir)
		{
		case CObject::LEFT:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX -= m_fSpeed;
			break;
		case CObject::RIGHT:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX += m_fSpeed;
			break;
		case CObject::UP:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fY -= m_fSpeed;
			break;
		case CObject::DOWN:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fY += m_fSpeed;
			break;
		case CObject::UP_LEFT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX -= diagonalSpeed;
			m_tInfo.fY -= diagonalSpeed;
			break;
		case CObject::UP_RIGHT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX += diagonalSpeed;
			m_tInfo.fY -= diagonalSpeed;
			break;
		case CObject::DOWN_LEFT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX -= diagonalSpeed;
			m_tInfo.fY += diagonalSpeed;
			break;
		case CObject::DOWN_RIGHT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX += diagonalSpeed;
			m_tInfo.fY += diagonalSpeed;
			break;
		case CObject::DIR_END:
			break;
		default:
			break;
		}
		m_fRollTime--;

		if (m_fRollTime < 0) {
			m_bIsRoll = false;
			m_fRollTime = 25.f;
			m_fFixScrollSpeed = m_fSpeed;
			m_fSpeed =3.f;
		}
	}
}

void CPlayer::Offset()
{
	int		iOffSetminX = 412;
	int		iOffSetmaxX = 612;

	int iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	if (iOffSetminX > m_tInfo.fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(m_fFixScrollSpeed);

	if (iOffSetmaxX < m_tInfo.fX + iScrollX)
		CScrollManager::Get_Instance()->Set_ScrollX(-m_fFixScrollSpeed);

	int		iOffSetminY = 260;
	int		iOffSetmaxY = 460;

	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (iOffSetminY > m_tInfo.fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(m_fFixScrollSpeed);

	if (iOffSetmaxY < m_tInfo.fY + iScrollY)
		CScrollManager::Get_Instance()->Set_ScrollY(-m_fFixScrollSpeed);
}

void CPlayer::Load_Assets()
{
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Down/will_walk_down.bmp", L"Will_Walk_down");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Left/will_walk_left.bmp", L"Will_Walk_left");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Right/will_walk_right.bmp", L"Will_Walk_right");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Up/will_walk_up.bmp", L"Will_Walk_up");

	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Idle/Down/will_idle_down.bmp", L"Will_Idle_down");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Idle/Left/will_idle_left.bmp", L"Will_Idle_left");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Idle/Right/will_idle_right.bmp", L"Will_Idle_right");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Idle/Up/will_idle_up.bmp", L"Will_Idle_up");

	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Roll/Down/will_roll_down.bmp", L"Will_Roll_down");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Roll/Left/will_roll_left.bmp", L"Will_Roll_left");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Roll/Right/will_roll_right.bmp", L"Will_Roll_right");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Roll/Up/will_roll_up.bmp", L"Will_Roll_up");
}

void CPlayer::Change_Motion()
{
	if ((m_ePreState != m_eCurState) || (m_ePreDir != m_eCurDir))
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			switch (m_eCurDir)
			{
			case CObject::LEFT:
				m_pImgKey = L"Will_Idle_left";
				break;
			case CObject::RIGHT:
				m_pImgKey = L"Will_Idle_right";
				break;
			case CObject::UP:
			case CObject::UP_LEFT:
			case CObject::UP_RIGHT:
				m_pImgKey = L"Will_Idle_up";
				break;
			case CObject::DOWN:
			case CObject::DOWN_LEFT:
			case CObject::DOWN_RIGHT:
				m_pImgKey = L"Will_Idle_down";
				break;
			case CObject::DIR_END:
				break;
			default:
				break;
			}
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::WALK:
			switch (m_eCurDir)
			{
			case CObject::LEFT:
				m_pImgKey = L"Will_Walk_left";
				break;
			case CObject::RIGHT:
				m_pImgKey = L"Will_Walk_right";
				break;
			case CObject::UP:
			case CObject::UP_LEFT:
			case CObject::UP_RIGHT:
				m_pImgKey = L"Will_Walk_up";
				break;
			case CObject::DOWN:
			case CObject::DOWN_LEFT:
			case CObject::DOWN_RIGHT:
				m_pImgKey = L"Will_Walk_down";
				break;
			case CObject::DIR_END:
				break;
			default:
				break;
			}
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CPlayer::ROLL:
			switch (m_eCurDir)
			{
			case CObject::LEFT:
				m_pImgKey = L"Will_Roll_left";
				break;
			case CObject::RIGHT:
				m_pImgKey = L"Will_Roll_right";
				break;
			case CObject::UP:
			case CObject::UP_LEFT:
			case CObject::UP_RIGHT:
				m_pImgKey = L"Will_Roll_up";
				break;
			case CObject::DOWN:
			case CObject::DOWN_LEFT:
			case CObject::DOWN_RIGHT:
				m_pImgKey = L"Will_Roll_down";
				break;
			case CObject::DIR_END:
				break;
			default:
				break;
			}
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount64();
			break;

		//case CPlayer::ATTACK:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 5;
		//	m_tFrame.dwSpeed = 200;
		//	m_tFrame.dwTime = GetTickCount64();
		//	break;

		//case CPlayer::HIT:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 1;
		//	m_tFrame.dwSpeed = 200;
		//	m_tFrame.dwTime = GetTickCount64();
		//	break;

		//case CPlayer::DEAD:
		//	m_tFrame.iFrameStart = 0;
		//	m_tFrame.iFrameEnd = 3;
		//	m_tFrame.dwSpeed = 200;
		//	m_tFrame.dwTime = GetTickCount64();
		//	break;
		}

		m_ePreState = m_eCurState;
		m_ePreDir = m_eCurDir;
	}
}


