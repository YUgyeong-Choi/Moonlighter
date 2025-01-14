#include "pch.h"
#include "CPlayer.h"
#include "CBitManager.h"
#include "CKeyManager.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"
#include "CUiManager.h"
#include "CItem.h"
#include "CAbstractFactory.h"
#include "CPlayerArrow.h"

CPlayer::CPlayer():m_bIsRoll(false), m_eCurState(STATE_END), m_ePreState(STATE_END), m_ePreDir(DIR_END), m_eCurDir(DIR_END), m_fRollTime(0), alpha(255), mbIsAttack(false), m_fComboTime(0), m_bFalling(false), m_bOnslime(false), m_bInvenOpen(false), m_iMoney(0), m_bUsePendant(false), m_NoRenderPlayer(false), m_SelectFirstWeapon(true), m_ArrowSpawn(false), m_bGetPotion(false), m_bCanPotionUse(true)
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

	m_PandentFrame.iFrameStart = 0;
	m_PandentFrame.iFrameEnd = 18;
	m_PandentFrame.dwSpeed = 100;
	m_PandentFrame.dwTime = GetTickCount64();

	m_PotionFrame.iFrameStart = 0;
	m_PotionFrame.iFrameEnd = 9;
	m_PotionFrame.dwSpeed = 100;
	m_PotionFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 80.f;
	m_tRenderSizeY = 80.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_iHp = 100;
	m_iMaxHp = m_iHp;
	m_iAttackDamage = 25;
	m_iHeal = 30;
	m_iMoney = 0;
}

int CPlayer::Update()
{
	if (m_bUsePendant) {
		if (m_PandentFrame.iFrameStart == 10) {
			m_NoRenderPlayer = true;
		}
		if (m_PandentFrame.iFrameStart == m_PandentFrame.iFrameEnd) {
			CSceneManager::GetInstance()->SetScene(SC_VILLAGE);
			m_bUsePendant = false;
			m_NoRenderPlayer = false;
			m_PandentFrame.iFrameStart = 0;
		}
	}

	if (m_bGetPotion) {
		if (m_PotionFrame.iFrameStart == m_PotionFrame.iFrameEnd) {
			m_bGetPotion = false;
			m_PotionFrame.iFrameStart = 0;
		}
	}

	if (m_iHp <= 0) {
		//return OBJ_DEAD;
	}
	Key_Input();
	Attack();
	Change_Motion();
	if (m_bOnslime) {
		m_fSpeed = 1.5f;
	}
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
	if (!m_bCanPotionUse) {
		m_iHp++;
		if (m_iHp > 100) {
			m_iHp = 100;
		}
		m_iHeal--;
		if (m_iHeal <= 0) {
			m_iHeal = 20;
			m_bCanPotionUse = true;
		}
	}
	SoundEffet();
	if (m_eCurState != FALL) {
		__super::Move_Frame();
	}
	
	if (m_bUsePendant) {
		if (m_PandentFrame.dwTime + m_PandentFrame.dwSpeed < GetTickCount64())
		{
			++m_PandentFrame.iFrameStart;

			if (m_PandentFrame.iFrameStart > m_PandentFrame.iFrameEnd)
				m_PandentFrame.iFrameStart = 0;

			m_PandentFrame.dwTime = GetTickCount64();
		}
	}

	if (m_bGetPotion) {
		if (m_PotionFrame.dwTime + m_PotionFrame.dwSpeed < GetTickCount64())
		{
			++m_PotionFrame.iFrameStart;

			if (m_PotionFrame.iFrameStart > m_PotionFrame.iFrameEnd)
				m_PotionFrame.iFrameStart = 0;

			m_PotionFrame.dwTime = GetTickCount64();
		}
	}
}

void CPlayer::Render(HDC hDC)
{
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	if (m_NoRenderPlayer) {
		Image* image;
		image = Image::FromFile(L"../MoonlighterAssets/Player/Pendant/will_pandent.png");
		graphics.DrawImage(image, (int)m_tInfo.fX - 90 + iScrollX, (int)m_tInfo.fY - 50 + iScrollY, (int)180 * m_PandentFrame.iFrameStart, 0, (int)180, (int)100, UnitPixel);
		delete image;
		return;
	}

	if (m_bGetPotion) {
		Image* image;
		image = Image::FromFile(L"../MoonlighterAssets/Player/Potion/get_potion.png");
		graphics.DrawImage(image, (int)m_tInfo.fX - 100 + iScrollX, (int)m_tInfo.fY - 100 + iScrollY, (int)200 * m_PotionFrame.iFrameStart, 0, (int)200, (int)200, UnitPixel);
		delete image;
	}

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
		if (m_SelectFirstWeapon) {
			if (CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == SWORD) {
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
			}
			else if (CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == BOW) {
				switch (m_eCurDir)
				{
				case LEFT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_left.png");
					break;
				case RIGHT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_right.png");
					break;
				case UP:
				case UP_LEFT:
				case UP_RIGHT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_up.png");
					break;
				case DOWN:
				case DOWN_LEFT:
				case DOWN_RIGHT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_down.png");
					break;
				}
			}
		}
		else {
			if (CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == SWORD) {
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
			}
			else if (CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == BOW) {
				switch (m_eCurDir)
				{
				case LEFT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_left.png");
					break;
				case RIGHT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_right.png");
					break;
				case UP:
				case UP_LEFT:
				case UP_RIGHT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_up.png");
					break;
				case DOWN:
				case DOWN_LEFT:
				case DOWN_RIGHT:
					image = Image::FromFile(L"../MoonlighterAssets/Weapon/Bow/will_bow_down.png");
					break;
				}
			}
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

	if (m_bUsePendant && !m_NoRenderPlayer) {
		Image* image;
		image = Image::FromFile(L"../MoonlighterAssets/Player/Pendant/will_pandent.png");
		graphics.DrawImage(image, (int)m_tInfo.fX - 90 + iScrollX, (int)m_tInfo.fY - 50 + iScrollY, (int)180 * m_PandentFrame.iFrameStart, 0, (int)180, (int)100, UnitPixel);
		delete image;
	}

	if (g_bDevmode) {
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		Attackbox(hDC, m_HitBox, iScrollX, iScrollY);
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);

		TCHAR szPlayer[64];
		_stprintf_s(szPlayer, _T("Player: X=%d, Y=%d, Hp=%d"), (int)m_tInfo.fX, (int)m_tInfo.fY, m_iHp);
		TCHAR szScroll[64];
		_stprintf_s(szScroll, _T("Scroll: X=%d, Y=%d"), (int)iScrollX, (int)iScrollY);
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, 300, 10, szPlayer, _tcslen(szPlayer));
		TextOut(hDC, 300, 25, szScroll, _tcslen(szScroll));
	}

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
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_damaged.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}
		}
		break;
	case OBJ_MONSTER_BULLET:
		if (!m_bIsRoll && m_bCanHit) {
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_damaged.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
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
	case OBJ_SLIME:
		m_bOnslime = true;
		break;
	case OBJ_PORTAL:
		break;
	case OBJ_ITEM:
		CUiManager::GetInstance()->AddItem(static_cast<CItem*>(_obj)->Get_ItemType());
		break;
	case OBJ_END:
		break;
	case OBJ_BOSS:
		if (!m_bIsRoll && m_bCanHit) {
			CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"will_damaged.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			if (m_fAttacktedTime + 500 < GetTickCount64()) {
				m_iAttackedDamage = _obj->Get_AttackDamage();
				m_bCanHit = false;
				m_fAttacktedTime = GetTickCount64();
			}
		}
		break;
	default:
		break;
	}
}

void CPlayer::Key_Input()
{
	if (m_eCurState != FALL) {
		if (CKeyManager::Get_Instance()->Key_Pressing('A') && CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant)
		{
			m_eCurState = IDLE;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('W') && CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant) {
			m_eCurState = IDLE;
		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('W') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant)
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
		else if (CKeyManager::Get_Instance()->Key_Pressing('S') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant)
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
		else if (CKeyManager::Get_Instance()->Key_Pressing('A') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant)
		{
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX -= m_fSpeed;
			m_eCurDir = LEFT;
			m_eCurState = WALK;

		}
		else if (CKeyManager::Get_Instance()->Key_Pressing('D') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant)
		{
			m_fFixScrollSpeed = m_fSpeed;
			m_tInfo.fX += m_fSpeed;
			m_eCurDir = RIGHT;
			m_eCurState = WALK;
		}
		else {
			if (!m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen) {
				m_eCurState = IDLE;
			}
		}

		if (CKeyManager::Get_Instance()->Key_Down(KEY_ATTACK,'K') && !m_bIsRoll && !m_bInvenOpen && !m_bUsePendant) {
			if (m_SelectFirstWeapon) {
				if (CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId != ITEM_END) {
					m_eCurState = ATTACK;
					mbIsAttack = true;
					m_ArrowSpawn = false;
				}
			}
			else {
				if (CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId != ITEM_END) {
					m_eCurState = ATTACK;
					mbIsAttack = true;
					m_ArrowSpawn = false;
				}
			}

		}

		if (CKeyManager::Get_Instance()->Key_Down(KEY_MOVE,VK_SPACE) && !(m_eCurState == ATTACK) && !m_bInvenOpen && !m_bUsePendant) {
			if (!m_bIsRoll) {
				m_bOnslime = false;
				m_bIsRoll = true;
				m_eCurState = ROLL;
				m_fSpeed = 5.f;
			}
		}

		if (CKeyManager::Get_Instance()->Key_Down(KEY_INVEN,'I') && !m_bIsRoll && !(m_eCurState == ATTACK)  && !m_bUsePendant) {
			m_bInvenOpen = !m_bInvenOpen;
			if (m_bInvenOpen) {
				CUiManager::GetInstance()->Set_UiType(UI_INVEN);
				static_cast<CInventory*>(CUiManager::GetInstance()->Get_Inven())->InitXY();
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"gui_inventory_open.wav", SOUND_EFFECT, g_fEffectVolume, true);
			}
			else {
				CUiManager::GetInstance()->Set_UiType(UI_END);
			}
		}

		if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, 'L') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen) {
			if (CSceneManager::GetInstance()->Get_SceneID() == SC_GOLEMDUNGEON || CSceneManager::GetInstance()->Get_SceneID() == SC_EDIT) {
				m_bUsePendant = true;
			}
		}

		if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, 'Z') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen) {
			m_SelectFirstWeapon = !m_SelectFirstWeapon;
			CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"will_weapon_change.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
		}

		if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, VK_F6 
		) && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen) {
			CUiManager::GetInstance()->AddItem(POTION);
			m_bGetPotion = true;
		}


		if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE, 'E') && !m_bIsRoll && !(m_eCurState == ATTACK) && !m_bInvenOpen) {
			if (CUiManager::GetInstance()->Get_Potion()->Get_Item().num > 0) {
				if (m_bCanPotionUse) {
					m_bCanPotionUse = false;
					CUiManager::GetInstance()->Get_Potion()->Sub_ItemNum();
					CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_potion_used.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
					if (CUiManager::GetInstance()->Get_Potion()->Get_Item().num == 0) {
						CUiManager::GetInstance()->Get_Potion()->Item_Init();
					}
				}
			}
		}
	}
}

void CPlayer::Rolling()
{
	if (m_bIsRoll) {
		m_bOnslime = false;
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
			m_tFrame.dwSpeed = 300;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CPlayer::ROLL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::ATTACK:
			if (m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == SWORD) {
				m_tFrame.iFrameEnd = 4;
			}
			if (!m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == SWORD) {
				m_tFrame.iFrameEnd = 4;
			}
			if (m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == BOW) {
				m_tFrame.iFrameEnd = 10;
			}
			if (!m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == BOW) {
				m_tFrame.iFrameEnd = 10;
			}

			m_tFrame.iFrameStart = 0;
			m_tFrame.dwSpeed = 70;
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
		case SC_GOLEMDUNGEON:
			if (m_bOnslime) {
				m_fTimeSinceLastStep += 0.1f;
				if (m_fTimeSinceLastStep >= 2.3) {
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_step_dungeon_slime.wav", SOUND_EFFECT, g_fPlayerVolume, true);
					m_fTimeSinceLastStep = 0;
				}
			}
			else {
				m_fTimeSinceLastStep += 0.1f;
				if (m_fTimeSinceLastStep >= 2.3) {
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_step_golem_dungeon.wav", SOUND_EFFECT, g_fPlayerVolume, true);
					m_fTimeSinceLastStep = 0;
				}
			}

			break;
		case SC_VILLAGE:
			m_fTimeSinceLastStep += 0.1f;
			if (m_fTimeSinceLastStep >= 2.3) {
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_step_town_gravel.wav", SOUND_EFFECT, g_fPlayerVolume, true);
				m_fTimeSinceLastStep = 0;
			}
			break;
		case SC_FIELD:
			m_fTimeSinceLastStep += 0.1f;
			if (m_fTimeSinceLastStep >= 2.3) {
				CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"will_step_town_pavement.wav", SOUND_EFFECT, g_fPlayerVolume, true);
				m_fTimeSinceLastStep = 0;
			}
			break;
		case SC_GOLEMBOSS:
			m_fTimeSinceLastStep += 0.1f;
			if (m_bOnslime) {
				if (m_fTimeSinceLastStep >= 4.6) {
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_step_dungeon_slime.wav", SOUND_EFFECT, g_fPlayerVolume, true);
					m_fTimeSinceLastStep = 0;
				}
			}
			else {
				if (m_fTimeSinceLastStep >= 2.3) {
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_step_golem_dungeon.wav", SOUND_EFFECT, g_fPlayerVolume, true);
					m_fTimeSinceLastStep = 0;
				}
			}
			break;
		case SC_EDIT:
			m_fTimeSinceLastStep += 0.1f;
			if (m_bOnslime) {
				if (m_fTimeSinceLastStep >= 4.6) {
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_step_dungeon_slime.wav", SOUND_EFFECT, g_fPlayerVolume, true);
					m_fTimeSinceLastStep = 0;
				}
			}
			else {
				if (m_fTimeSinceLastStep >= 4.6) {
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"will_step_golem_dungeon.wav", SOUND_EFFECT, g_fPlayerVolume, true);
					m_fTimeSinceLastStep = 0;
				}
			}
			break;
		}

		break;

	case CPlayer::ROLL:
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"will_roll.wav", SOUND_EFFECT, g_fPlayerVolume, true);
		break;
	case CPlayer::FALL:
		CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
		CSoundManager::Get_Instance()->PlaySound(L"will_fall.wav", SOUND_EFFECT, g_fPlayerVolume, true);
		break;
	case CPlayer::ATTACK:
		if (m_SelectFirstWeapon&& CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == BOW) {
			if (m_tFrame.iFrameStart == 2) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"bow_main_attack_shoot.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}

			if (m_tFrame.iFrameStart == 4) {
				if (!m_ArrowSpawn) {
					switch (m_eCurDir)
					{
					case LEFT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX - 10, m_tInfo.fY, LEFT));
						break;
					case RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX + 10, m_tInfo.fY, RIGHT));
						break;
					case UP:
					case UP_LEFT:
					case UP_RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX, m_tInfo.fY - 10, UP));
						break;
					case DOWN:
					case DOWN_LEFT:
					case DOWN_RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX, m_tInfo.fY + 10, DOWN));
						break;
					}
					m_ArrowSpawn = true;
				}
			}
		}
		if (!m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == BOW) {
			if (m_tFrame.iFrameStart == 2) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"bow_main_attack_shoot.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}
			if (m_tFrame.iFrameStart == 4) {
				if (!m_ArrowSpawn) {
					switch (m_eCurDir)
					{
					case LEFT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX - 10, m_tInfo.fY, LEFT));
						break;
					case RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX + 10, m_tInfo.fY, RIGHT));
						break;
					case UP:
					case UP_LEFT:
					case UP_RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX, m_tInfo.fY - 10, UP));
						break;
					case DOWN:
					case DOWN_LEFT:
					case DOWN_RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX, m_tInfo.fY + 10, DOWN));
						break;
					}
					m_ArrowSpawn = true;
				}
			}
		}

		if (m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == BOW) {
			if (m_tFrame.iFrameStart == 2) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"bow_main_attack_shoot.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}

			if (m_tFrame.iFrameStart == 4) {
				if (!m_ArrowSpawn) {
					switch (m_eCurDir)
					{
					case LEFT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX - 10, m_tInfo.fY, LEFT));
						break;
					case RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX + 10, m_tInfo.fY, RIGHT));
						break;
					case UP:
					case UP_LEFT:
					case UP_RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX, m_tInfo.fY - 10, UP));
						break;
					case DOWN:
					case DOWN_LEFT:
					case DOWN_RIGHT:
						CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER_ARROW, CAbstractFactory<CPlayerArrow>::Create(m_tInfo.fX, m_tInfo.fY + 10, DOWN));
						break;
					}
					m_ArrowSpawn = true;
				}
			}
		}
		if (!m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == SWORD) {

			if (4 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-1);
			}
			if (5 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(1);
			}
			if (6 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-1);
			}
			if (7 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(1);
			}
			if (8 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-1);
			}
			if (9 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(1);
			}

			if (11 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-2);
			}
			if (12 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(2);
			}
			if (13 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-2);
			}
			if (14 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(2);
			}
			if (15 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-2);
			}
			if (16 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(2);
			}
		}


		if (m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == SWORD) {
			if (4 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-1);
			}
			if (5 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(1);
			}
			if (6 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-1);
			}
			if (7 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(1);
			}
			if (8 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-1);
			}
			if (9 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(1);
			}

			if (11 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-2);
			}
			if (12 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(2);
			}
			if (13 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-2);
			}
			if (14 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(2);
			}
			if (15 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(-2);
			}
			if (16 == m_tFrame.iFrameStart) {
				CScrollManager::Get_Instance()->Set_ScrollX(2);
			}
		}

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
	if (m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == SWORD) {
		if (mbIsAttack) {
			switch (m_eCurDir)
			{
			case LEFT:
				m_HitBox = { (long)m_tInfo.fX - 60,(long)m_tInfo.fY - 50,(long)m_tInfo.fX,(long)m_tInfo.fY + 50 };
				break;
			case RIGHT:
				m_HitBox = { (long)m_tInfo.fX,(long)m_tInfo.fY - 50,(long)m_tInfo.fX + 60,(long)m_tInfo.fY + 50 };
				break;
			case UP:
			case UP_LEFT:
			case UP_RIGHT:
				m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY - 60 ,(long)m_tInfo.fX + 50,(long)m_tInfo.fY };
				break;
			case DOWN:
			case DOWN_LEFT:
			case DOWN_RIGHT:
				m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY,(long)m_tInfo.fX + 50,(long)m_tInfo.fY + 60 };
				break;
			}

			m_tRenderSizeX = 200.f;
			m_tRenderSizeY = 200.f;
			if (m_tFrame.iFrameStart == 0) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing1.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}


			if (2 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart < 4) {
				m_tFrame.iFrameEnd = 8;
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing3.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}

			if (6 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart < 8) {
				m_tFrame.iFrameEnd = 17;
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing3.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}
		}
	}

	if (!m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == SWORD) {
		if (mbIsAttack) {
			switch (m_eCurDir)
			{
			case LEFT:
				m_HitBox = { (long)m_tInfo.fX - 60,(long)m_tInfo.fY - 50,(long)m_tInfo.fX,(long)m_tInfo.fY + 50 };
				break;
			case RIGHT:
				m_HitBox = { (long)m_tInfo.fX,(long)m_tInfo.fY - 50,(long)m_tInfo.fX + 60,(long)m_tInfo.fY + 50 };
				break;
			case UP:
			case UP_LEFT:
			case UP_RIGHT:
				m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY - 60 ,(long)m_tInfo.fX + 50,(long)m_tInfo.fY };
				break;
			case DOWN:
			case DOWN_LEFT:
			case DOWN_RIGHT:
				m_HitBox = { (long)m_tInfo.fX - 50,(long)m_tInfo.fY,(long)m_tInfo.fX + 50,(long)m_tInfo.fY + 60 };
				break;
			}
			if (m_tFrame.iFrameStart == 0) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing1.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
			}

			m_tRenderSizeX = 200.f;
			m_tRenderSizeY = 200.f;

			if (2 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart < 4) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing2.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
				m_tFrame.iFrameEnd = 8;
			}

			if (6 <= m_tFrame.iFrameStart && m_tFrame.iFrameStart < 8) {
				CSoundManager::Get_Instance()->StopSound(PLAYER_EFFECT);
				CSoundManager::Get_Instance()->PlaySound(L"short_sword_main_swing3.wav", PLAYER_EFFECT, g_fPlayerVolume, true);
				m_tFrame.iFrameEnd = 17;
			}
		}
	}

	if (m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon1()->Get_Item().itemId == BOW) {
		if (mbIsAttack) {
			m_tRenderSizeX = 200.f;
			m_tRenderSizeY = 200.f;
		}
	}

	if (!m_SelectFirstWeapon && CUiManager::GetInstance()->Get_Wepon2()->Get_Item().itemId == BOW) {
		if (mbIsAttack) {
			m_tRenderSizeX = 200.f;
			m_tRenderSizeY = 200.f;
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


void CPlayer::Set_State(int hp, int money, int maxHp)
{
	m_iHp = hp;
	m_iMaxHp = maxHp;
	m_iMoney = money;
}