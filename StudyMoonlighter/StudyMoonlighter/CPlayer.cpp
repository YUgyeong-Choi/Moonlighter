#include "pch.h"
#include "CPlayer.h"
#include "CBitManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"

CPlayer::CPlayer():m_bIsRoll(false), m_eCurState(STATE_END), m_ePreState(STATE_END), m_ePreDir(DIR_END), m_eCurDir(DIR_END), m_fRollTime(0), image(nullptr), alpha(255)
{
}

void CPlayer::Initialize()
{
	m_eOBJID = OBJ_PLAYER;

	m_tInfo.fCX = 37.f;
	m_tInfo.fCY = 54.f;

	m_fSpeed = 3.f;
	m_fFixScrollSpeed = 3.f;
	m_fRollTime = 30.f;

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
	if (m_eCurState == FALL) {
		alpha -= 16;
		if (alpha < 0) {
			switch (CObjectManager::Get_Instance()->Get_MapDir())
			{
			case LEFT:
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * CObjectManager::Get_Instance()->Get_MapXIndex()) + 900, WINCY / 2);
				break;
			case RIGHT:
				CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * CObjectManager::Get_Instance()->Get_MapXIndex()) + 150, WINCY / 2);
				break;
			case UP:
				break;
			case DOWN:
				break;
			default:
				break;
			}
			m_eCurState = IDLE;
			alpha = 255;
		}
	}
	__super::Update_Rect();
	return 0;
}

void CPlayer::Late_Update()
{
	Rolling();
	if (m_eCurState != FALL) {
		__super::Move_Frame();
	}
}

void CPlayer::Render(HDC hDC)
{
	
	switch (m_eCurState)
	{
	case CPlayer::IDLE:
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Idle/Left/will_idle_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Idle/Right/will_idle_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Idle/Up/will_idle_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Idle/Down/will_idle_down.png");
			break;
		}
		break;
	case CPlayer::WALK:
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Walk/Left/will_walk_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Walk/Right/will_walk_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Walk/Up/will_walk_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Walk/Down/will_walk_down.png");
			break;
		}
		break;
	case CPlayer::ROLL:
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Roll/Left/will_roll_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Roll/Right/will_roll_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Roll/Up/will_roll_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Player/Roll/Down/will_roll_down.png");
			break;
		}
		break;
	case CPlayer::FALL:
		break;
	case CPlayer::ATTACK:
		break;
	case CPlayer::HIT:
		break;
	case CPlayer::DEAD:
		break;
	case CPlayer::STATE_END:
		break;
	default:
		break;
	}

	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (m_eCurState != FALL) {
		graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);
	}
	else {
		ImageAttributes imgAttrs;
		ColorMatrix colorMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, alpha / 255.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		imgAttrs.SetColorMatrix(&colorMatrix);
		graphics.DrawImage(image,
				Gdiplus::Rect(
				(int)m_tRenderRect.left + iScrollX,
				(int)m_tRenderRect.top + iScrollY,
				m_tRenderSizeX,
				m_tRenderSizeY),
			(int)m_tRenderSizeX* m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, Gdiplus::UnitPixel, &imgAttrs);

	}
	
	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, _T("Player: X=%d, Y=%d"), (int)m_tInfo.fX, (int)m_tInfo.fY);
	//_stprintf_s(szBuffer, _T("Player: X=%d, Y=%d"), (int)iScrollX, (int)iScrollY);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 10, 10, szBuffer, _tcslen(szBuffer));

}

void CPlayer::Release()
{
}

void CPlayer::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_FLOOR) {
		m_eCurState = FALL;

	}
}

void CPlayer::Key_Input()
{
	if (m_eCurState != FALL) {
		if (CKeyManager::Get_Instance()->Key_Pressing('A') && CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll)
		{
			m_eCurState = IDLE;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('W') && CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll) {
			m_eCurState = IDLE;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('W') && !m_bIsRoll)
		{
			if (CKeyManager::Get_Instance()->Key_Pressing('D')) {

				float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
				m_fFixScrollSpeed = diagonalSpeed;
				m_tInfo.fX += diagonalSpeed;
				m_tInfo.fY -= diagonalSpeed;
				m_eCurDir = UP_RIGHT;
			}
			else if (CKeyManager::Get_Instance()->Key_Pressing('A')) {
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

		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll)
		{
			if (CKeyManager::Get_Instance()->Key_Pressing('D')) {
				float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
				m_fFixScrollSpeed = diagonalSpeed;
				m_tInfo.fX += diagonalSpeed;
				m_tInfo.fY += diagonalSpeed;
				m_eCurDir = DOWN_RIGHT;
			}
			else if (CKeyManager::Get_Instance()->Key_Pressing('A')) {
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


		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('A') && !m_bIsRoll)
		{
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX -= m_fSpeed;
			m_eCurDir = LEFT;
			m_eCurState = WALK;

		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll)
		{
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX += m_fSpeed;
			m_eCurDir = RIGHT;
			m_eCurState = WALK;
		}
		else {
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
}

void CPlayer::Rolling()
{
	if (m_bIsRoll) {
		float diagonalSpeed = m_fSpeed / (float)sqrt(2.0f);
		switch (m_eCurDir)
		{
		case LEFT:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX -= m_fSpeed;
			break;
		case RIGHT:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX += m_fSpeed;
			break;
		case UP:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fY -= m_fSpeed;
			break;
		case DOWN:
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fY += m_fSpeed;
			break;
		case UP_LEFT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX -= diagonalSpeed;
			m_tInfo.fY -= diagonalSpeed;
			break;
		case UP_RIGHT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX += diagonalSpeed;
			m_tInfo.fY -= diagonalSpeed;
			break;
		case DOWN_LEFT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX -= diagonalSpeed;
			m_tInfo.fY += diagonalSpeed;
			break;
		case DOWN_RIGHT:
			m_fFixScrollSpeed = diagonalSpeed;
			m_tInfo.fX += diagonalSpeed;
			m_tInfo.fY += diagonalSpeed;
			break;
		case DIR_END:
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

void CPlayer::Change_Motion()
{
	if ((m_ePreState != m_eCurState) || (m_ePreDir != m_eCurDir))
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CPlayer::ROLL:
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


