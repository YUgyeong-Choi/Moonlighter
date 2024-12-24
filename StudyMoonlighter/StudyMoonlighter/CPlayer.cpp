#include "pch.h"
#include "CPlayer.h"
#include "CBitManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"

CPlayer::CPlayer():m_bIsRoll(false), m_eCurState(STATE_END), m_ePreState(STATE_END), m_ePreDir(DIR_END), m_eCurDir(DIR_END), m_fRollTime(0), alpha(255), mbIsAttack(false), m_fComboTime(0), m_bFalling(false)
{
}

void CPlayer::Initialize()
{
	m_eOBJID = OBJ_PLAYER;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 45.f;

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
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 80.f;
	m_tRenderSizeY = 80.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_iHp = 100;
	m_iAttackDamage = 10;
}

int CPlayer::Update()
{
	if (m_iHp <= 0) {
		//return OBJ_DEAD;
	}
	Key_Input();
	Attack();
	Change_Motion();
	if (m_eCurState == FALL) {
		alpha -= 60;
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
			CObjectManager::Get_Instance()->Get_Player()->Set_Pos((1024 * CObjectManager::Get_Instance()->Get_MapXIndex()) + 150, WINCY / 2);
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
	Hit();
	SoundEffet();
	if (m_eCurState != FALL) {
		__super::Move_Frame();
	}
	
}

void CPlayer::Render(HDC hDC)
{
	Image* image(nullptr);
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
		switch (m_eCurDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Weapon/ShortSword/will_shortsword_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Weapon/ShortSword/will_shortsword_right.png");
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Weapon/ShortSword/will_shortsword_up.png");
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Weapon/ShortSword/will_shortsword_down.png");
			break;
		}
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
				(int)m_tRenderSizeX,
				(int)m_tRenderSizeY),
			(int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, Gdiplus::UnitPixel, &imgAttrs);

	}

	if (!m_bCanHit) {
		ImageAttributes imgAttrs;
		ColorMatrix colorMatrix;
		if (m_iAttackedDamage % 2 == 0) {
			colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Red channel
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // Green channel
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Blue channel
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Alpha channel
				1.0f, 1.0f, 1.0f, 0.0f, 1.0f   // Set translation to add white color
			};

		}
		else {
			colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Red channel
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Green channel (set to 0 to remove green)
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Blue channel (set to 0 to remove blue)
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Alpha channel (no change to transparency)
				1.0f, 0.0f, 0.0f, 0.0f, 1.0f   // Translation to add red color
			};
		}
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
		Hitbox(hDC, m_HitBox, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
	}

	TCHAR szBuffer[64];
	_stprintf_s(szBuffer, _T("Player: X=%d, Y=%d, Hp=%d"), (int)m_tInfo.fX, (int)m_tInfo.fY, m_iHp);
	//_stprintf_s(szBuffer, _T("Player: X=%d, Y=%d"), (int)iScrollX, (int)iScrollY);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 10, 10, szBuffer, _tcslen(szBuffer));

	delete image;
}

void CPlayer::Release()
{
}

void CPlayer::OnCollision(CObject* _obj)
{
	switch (_obj->Get_OBJID())
	{
	case OBJ_PLAYER:
		break;
	case OBJ_MONSTER:
		if (!m_bIsRoll && m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = 5;
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
			}
		}
		break;
	case OBJ_MONSTER_BULLET:
		if (!m_bIsRoll && m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
			}
		}
		break;
	case OBJ_MAPOBJ:
		break;
	case OBJ_FLOOR:
		if (m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = 5;
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				m_eCurState = FALL;
			}
		}
		break;
	case OBJ_PORTAL:
		break;
	case OBJ_END:
		break;
	default:
		break;
	}
}

void CPlayer::Key_Input()
{
	if (m_eCurState != FALL) {
		if (CKeyManager::Get_Instance()->Key_Pressing('A') && CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll && !(m_eCurState == ATTACK))
		{
			m_eCurState = IDLE;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('W') && CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll && !(m_eCurState == ATTACK)) {
			m_eCurState = IDLE;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('W') && !m_bIsRoll && !(m_eCurState == ATTACK))
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
		else if (CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll && !(m_eCurState == ATTACK))
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
		else if (CKeyManager::Get_Instance()->Key_Pressing('A') && !m_bIsRoll && !(m_eCurState == ATTACK))
		{
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX -= m_fSpeed;
			m_eCurDir = LEFT;
			m_eCurState = WALK;

		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll && !(m_eCurState == ATTACK))
		{
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX += m_fSpeed;
			m_eCurDir = RIGHT;
			m_eCurState = WALK;
		}
		else {
			if (!m_bIsRoll && !(m_eCurState == ATTACK)) {
				m_eCurState = IDLE;
			}
		}

		if (CKeyManager::Get_Instance()->Key_Down('K') && !m_bIsRoll) {
			m_eCurState = ATTACK;
			mbIsAttack = true;
		}

		if (CKeyManager::Get_Instance()->Key_Down(VK_SPACE) && !(m_eCurState == ATTACK)) {
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

		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

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

void CPlayer::SoundEffet()
{
	switch (m_eCurState)
	{
	case CPlayer::IDLE:
		break;
	case CPlayer::WALK:
		switch (CSceneManager::GetInstance()->Get_SceneID())
		{
		case SC_TUTORIAL:
			m_fTimeSinceLastStep += 0.1f;
			if (m_fTimeSinceLastStep >= 2.3) {
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_step_golem_dungeon.wav", SOUND_EFFECT, 0.1f, true);
				m_fTimeSinceLastStep = 0;
			}
			break;
		case SC_VILLAGE:
			m_fTimeSinceLastStep += 0.1f;
			if (m_fTimeSinceLastStep >= 2.3) {
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_step_town_gravel.wav", SOUND_EFFECT, 0.1f, true);
				m_fTimeSinceLastStep = 0;
			}
			break;
		case SC_FIELD:
			m_fTimeSinceLastStep += 0.1f;
			if (m_fTimeSinceLastStep >= 2.3) {
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_step_town_pavement.wav", SOUND_EFFECT, 0.1f, true);
				m_fTimeSinceLastStep = 0;
			}
			break;
		case SC_GOLEMBOSS:
			m_fTimeSinceLastStep += 0.1f;
			if (m_fTimeSinceLastStep >= 2.3) {
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_step_golem_dungeon.wav", SOUND_EFFECT, 0.1f, true);
				m_fTimeSinceLastStep = 0;
			}
			break;
		}
		break;
	case CPlayer::ROLL:
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"will_roll.wav", SOUND_EFFECT, g_fEffectVolume - 0.3f, true);
		break;
	case CPlayer::FALL:
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"will_fall.wav", SOUND_EFFECT, g_fEffectVolume - 0.3f, true);
		break;
	case CPlayer::ATTACK:
		break;
	case CPlayer::HIT:
		break;
	case CPlayer::DEAD:
		break;
	case CPlayer::STATE_END:
		break;
	}
}

void CPlayer::Attack()
{
	if (mbIsAttack) {
		switch (m_eCurDir)
		{
		case LEFT:
			m_HitBox = { (long)m_tInfo.fX - 60,(long)m_tInfo.fY - 50,(long)m_tInfo.fX,(long)m_tInfo.fY + 50 };
			break;
		case RIGHT:
			m_HitBox = { (long)m_tInfo.fX,(long)m_tInfo.fY-50,(long)m_tInfo.fX + 60,(long)m_tInfo.fY + 50 };
			break;
		case UP:
		case UP_LEFT:
		case UP_RIGHT:
			m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY - 60 ,(long)m_tInfo.fX + 50,(long)m_tInfo.fY  };
			break;
		case DOWN:
		case DOWN_LEFT:
		case DOWN_RIGHT:
			m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY,(long)m_tInfo.fX + 50,(long)m_tInfo.fY + 60 };
			break;
		}
		
		if (m_tFrame.iFrameStart == 0) {
			CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing1.wav", PLAYER_EFFECT, g_fEffectVolume, true);
		}
		m_tRenderSizeX = 200.f;
		m_tRenderSizeY = 200.f;

		if (2 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart < 4) {
			m_tFrame.iFrameEnd = 8;
			CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing2.wav", PLAYER_EFFECT, g_fEffectVolume, true);
		}

		if (6 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart < 8) {
			m_tFrame.iFrameEnd = 17;
			CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing3.wav", PLAYER_EFFECT, g_fEffectVolume, true);
		}
	}

	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
		m_tRenderSizeX = 80.f;
		m_tRenderSizeY = 80.f;
		m_eCurState = IDLE;
		m_HitBox = { 0,0,0,0 };
	}

	mbIsAttack = false;
}


