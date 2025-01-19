#include "pch.h"
#include "CShopPlayer.h"
#include "CObjectManager.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CUiManager.h"
#include "CInventory.h"
#include "CSoundManager.h"
#include "CInventoryManager.h"

CShopPlayer::CShopPlayer():m_iMoney(0), m_eCurState(STATE_END), m_ePreState(STATE_END), m_bIsRoll(false), m_bInvenOpen(false)
{
}

void CShopPlayer::Initialize()
{
	
    m_eOBJID = OBJ_PLAYER;
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 45.f;

	m_fSpeed = 4.f;
	m_fFixScrollSpeed = 4.f;
	m_fRollTime = 30.f;

	//그래픽 관련
	m_eCurState = IDLE;
	m_ePreDir = UP;
	m_eCurDir = UP;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 80.f;
	m_tRenderSizeY = 80.f;
	m_eRender = RENDER_GAMEOBJECT;
}

int CShopPlayer::Update()
{
	Key_Input();
	Change_Motion();
	SoundEffet();
	__super::Update_Rect();
    return 0;
}

void CShopPlayer::Late_Update()
{
	Rolling();
	__super::Move_Frame();
}

void CShopPlayer::Render(HDC hDC)
{
	Image* image(nullptr);
	switch (m_eCurState)
	{
	case CShopPlayer::IDLE:
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Idle/shop_idle_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Idle/shop_idle_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Idle/shop_idle_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Idle/shop_idle_down.png");
			break;
		}
		break;
	case CShopPlayer::WALK:
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Walk/shop_walk_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Walk/shop_walk_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Walk/shop_walk_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Walk/shop_walk_down.png");
			break;
		}
		break;
	case CShopPlayer::ROLL:
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Roll/shop_roll_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Roll/shop_roll_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Roll/shop_roll_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/ShopPlayer/Roll/shop_roll_down.png");
			break;
		}
		break;
	}

	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);

		TCHAR szPlayer[64];
		_stprintf_s(szPlayer, _T("Player: X=%d, Y=%d, Hp=%d"), (int)m_tInfo.fX, (int)m_tInfo.fY, m_iHp);
		TCHAR szScroll[64];
		_stprintf_s(szScroll, _T("Player: X=%d, Y=%d"), (int)iScrollX, (int)iScrollY);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, 300, 10, szPlayer, _tcslen(szPlayer));
		TextOut(hDC, 300, 25, szScroll, _tcslen(szScroll));
	}

	delete image;
}

void CShopPlayer::Release()
{
}

void CShopPlayer::Set_State(int hp, int money, int maxHp)
{
    m_iHp = hp;
    m_iMaxHp = maxHp;
    m_iMoney = money;
}

void CShopPlayer::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Pressing('A') && CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll && !m_bInvenOpen)
	{
		m_eCurState = IDLE;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing('W') && CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll && !m_bInvenOpen) {
		m_eCurState = IDLE;
	}
	else if (CKeyManager::Get_Instance()->Key_Pressing('W') && !m_bIsRoll && !m_bInvenOpen)
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
	else if (CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll && !m_bInvenOpen)
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
	else if (CKeyManager::Get_Instance()->Key_Pressing('A') && !m_bIsRoll && !m_bInvenOpen)
	{
		m_fFixScrollSpeed = m_fSpeed;
		m_tInfo.fX -= m_fSpeed;
		m_eCurDir = LEFT;
		m_eCurState = WALK;

	}
	else if (CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll && !m_bInvenOpen)
	{
		m_fFixScrollSpeed = m_fSpeed;
		m_tInfo.fX += m_fSpeed;
		m_eCurDir = RIGHT;
		m_eCurState = WALK;
	}
	else {
		if (!m_bIsRoll  && !m_bInvenOpen) {
			m_eCurState = IDLE;
		}
	}


	if (CKeyManager::Get_Instance()->Key_Down(KEY_MOVE,VK_SPACE) && !m_bInvenOpen) {
		if (!m_bIsRoll) {
			m_bIsRoll = true;
			m_eCurState = ROLL;
			m_fSpeed = 5.f;
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'I')) {
		m_bInvenOpen = !m_bInvenOpen;
		if (m_bInvenOpen) {
			CUiManager::GetInstance()->Set_UiType(UI_INVEN);
			static_cast<CInventory*>(CInventoryManager::GetInstance()->Get_Inven())->InitXY();
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_inventory_open.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
		else {
			CUiManager::GetInstance()->Set_UiType(UI_END);
		}
	}

}

void CShopPlayer::Rolling()
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
			m_fSpeed = 4.f;
		}
	}
}

void CShopPlayer::Change_Motion()
{
	if ((m_ePreState != m_eCurState) || (m_ePreDir != m_eCurDir))
	{
		switch (m_eCurState)
		{
		case CShopPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.dwSpeed = 300;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CShopPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CShopPlayer::ROLL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
		m_ePreDir = m_eCurDir;
	}
}

void CShopPlayer::SoundEffet()
{
	switch (m_eCurState)
	{
	case CShopPlayer::IDLE:
		break;
	case CShopPlayer::WALK:
		m_fTimeSinceLastStep += 0.1f;
		if (m_fTimeSinceLastStep >= 2.3) {
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"will_step_town_wood2.wav", SOUND_EFFECT, g_fPlayerVolume, true);
			m_fTimeSinceLastStep = 0;
		}
		break;
	case CShopPlayer::ROLL:
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"will_roll.wav", SOUND_EFFECT, g_fPlayerVolume, true);
		break;
	}
}
