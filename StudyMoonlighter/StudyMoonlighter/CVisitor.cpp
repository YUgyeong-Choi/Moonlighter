#include "pch.h"
#include "CVisitor.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CShopPlayer.h"
#include "CUiManager.h"
#include "CSceneManager.h"
#include "CSoundManager.h"
#include "CShopScene.h"
#include "CInventoryManager.h"

CVisitor::CVisitor():m_eCurPattern(CVisitor::END), m_ePrePattern(CVisitor::END), m_fSellX(0), m_fSellY(0), m_fCounterX(0), m_fCounterY(0), m_bSellXY(false), tick(0), m_itemState(ITEMSTATE_END), m_itemTalk(false), m_bTableXY(false), m_fBeforeCounterX(0), m_fBeforeCounterY(0), m_bBeforeTableXY(false), m_AddMoney(false), m_SellFinish(false), m_sound(false)
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

	m_fCounterX = 630;
	m_fCounterY = 600;

	m_fBeforeCounterX = 580;
	m_fBeforeCounterY = 650;

	m_iHp = 1;
}

int CVisitor::Update()
{
	if (m_iHp <= 0) {
		return OBJ_DEAD;
	}
	if (m_bSellXY && !m_itemTalk) {
		tick++;
		if (tick > 330) {
			m_itemTalk = true;
			m_itemFrame.dwTime = GetTickCount64();

		}
	}

	if (m_itemTalk) {
		if (m_itemFrame.iFrameStart == m_itemFrame.iFrameEnd) {
			m_bBeforeTableXY = true;
			m_eCurPattern = CVisitor::WALK;
			if (!m_sound) {
				switch (m_itemState)
				{
				case CVisitor::CHEAP:
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"visitor_talk_too_cheap.wav", SOUND_EFFECT, g_fEffectVolume, true);
					break;
				case CVisitor::GOOD:
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"visitor_talk_cheap.wav", SOUND_EFFECT, g_fEffectVolume, true);
					break;
				case CVisitor::BAD:
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"visitor_talk_expensive.wav", SOUND_EFFECT, g_fEffectVolume, true);
					break;
				case CVisitor::ITEMSTATE_END:
					break;
				default:
					break;
				}
				m_sound = true;

				if (m_itemState == BAD) {
					m_SellFinish = true;
				}
				else {
					CInventoryManager::GetInstance()->Get_InvenShop()->Set_InitSlot(m_index);
				}
				static_cast<CShopScene*>(CSceneManager::GetInstance()->Get_Scene())->Set_NpcCanSpawn(m_index);
			}

		}
	}

	Move_Npc();
	Change_Motion();
	__super::Update_Rect();
    return 0;
}

void CVisitor::Late_Update()
{
	__super::Move_Frame();

	if (m_itemTalk) {
		if (m_itemFrame.dwTime + m_itemFrame.dwSpeed < GetTickCount64())
		{
			++m_itemFrame.iFrameStart;

			if (m_itemFrame.iFrameStart > m_itemFrame.iFrameEnd)
				m_itemFrame.iFrameStart = 0;

			m_itemFrame.dwTime = GetTickCount64();
		}
	}
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

	if (m_bTableXY) {
		m_eDir = UP;
		m_eCurPattern = CVisitor::IDLE;
		if (!m_AddMoney) {
			static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Money(m_item.num * price);
			CUiManager::GetInstance()->Get_Moenyui();
			m_AddMoney = true;
			m_SellFinish = true;
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"shop_item_sold.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}
}

void CVisitor::Set_Init(int index, float _outx, float _outy, float _x, float _y, ITEM _item, int _price)
{
	m_index = index;
	m_fSellX = _x; 
	m_fSellY = _y; 
	m_item = _item;
	price = _price;
	m_fOutX = _outx;
	m_fOutY = _outy + 30;

	if (_price < _item.price * 0.5) {
		m_itemState = CHEAP;
	}
	else if (_price > _item.price * 1.5) {
		m_itemState = BAD;
	}
	else {
		m_itemState = GOOD;
	}

	switch (m_itemState)
	{
	case CVisitor::CHEAP:
		m_itemFrame.iFrameStart = 0;
		m_itemFrame.iFrameEnd = 10;
		m_itemFrame.dwSpeed = 100;
		break;
	case CVisitor::GOOD:
		m_itemFrame.iFrameStart = 0;
		m_itemFrame.iFrameEnd = 1;
		m_itemFrame.dwSpeed = 500;
		break;
	case CVisitor::BAD:
		m_itemFrame.iFrameStart = 0;
		m_itemFrame.iFrameEnd = 6;
		m_itemFrame.dwSpeed = 150;
		break;
	case CVisitor::ITEMSTATE_END:
		break;
	default:
		break;
	}
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

	if (m_bTableXY && !m_SellFinish) {
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

		fWidth = m_fCounterX - m_tInfo.fX;
		fHeight = m_fCounterY - m_tInfo.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);

		m_fAngle = fRadian * (180.f / PI);

		if (m_fCounterY > m_tInfo.fY)
			m_fAngle *= -1.f;

		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


		float x = m_fCounterX - m_tInfo.fX;
		float y = m_fCounterY - m_tInfo.fY;


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

	if (m_bBeforeTableXY&& !m_bTableXY &&!m_SellFinish) {
		if (m_fBeforeCounterX - 10 <= m_tInfo.fX && m_tInfo.fX <= m_fBeforeCounterX + 10) {
			if (m_fBeforeCounterY - 10 <= m_tInfo.fY && m_tInfo.fY <= m_fBeforeCounterY + 10) {
				m_bTableXY = true;
				return;
			}
		}
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

		fWidth = m_fBeforeCounterX - m_tInfo.fX;
		fHeight = m_fBeforeCounterY - m_tInfo.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);

		m_fAngle = fRadian * (180.f / PI);

		if (m_fBeforeCounterY > m_tInfo.fY)
			m_fAngle *= -1.f;

		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


		float x = m_fBeforeCounterX - m_tInfo.fX;
		float y = m_fBeforeCounterY - m_tInfo.fY;


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

	if (m_SellFinish) {
		if (m_fOutX - 10 <= m_tInfo.fX && m_tInfo.fX <= m_fOutX +10 ) {
			if (m_fOutY - 10<= m_tInfo.fY && m_tInfo.fY <= m_fOutY + 10) {
				m_iHp = 0;
				return;
			}
		}
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

		fWidth = m_fOutX - m_tInfo.fX;
		fHeight = m_fOutY - m_tInfo.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);

		m_fAngle = fRadian * (180.f / PI);

		if (m_fOutY > m_tInfo.fY)
			m_fAngle *= -1.f;

		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));


		float x = m_fOutX - m_tInfo.fX;
		float y = m_fOutY - m_tInfo.fY;


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

	if (!m_itemTalk) {
		if (100 <= tick && tick < 180) {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/visitortalk_thinkcycle_1.png");
		}
		else if (180 <= tick && tick < 260) {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/visitortalk_thinkcycle_2.png");
		}
		else if (260 <= tick) {
			image = Image::FromFile(L"../MoonlighterAssets/Ui/visitortalk_thinkcycle_3.png");
		}
		graphics.DrawImage(image, (int)m_tInfo.fX + 20 + iScrollX, (int)m_tInfo.fY - 40 + iScrollY, 0, 0, 40, 16, UnitPixel);
	}
	else {
		switch (m_itemState)
		{
		case CVisitor::CHEAP:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/talk_cheap.png");
			break;
		case CVisitor::GOOD:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/talk_good.png");
			break;
		case CVisitor::BAD:
			image = Image::FromFile(L"../MoonlighterAssets/Ui/talk_bad.png");
			break;
		case CVisitor::ITEMSTATE_END:
			break;
		default:
			break;
		}
		graphics.DrawImage(image, (int)m_tInfo.fX + 20 + iScrollX, (int)m_tInfo.fY - 50 + iScrollY, m_itemFrame.iFrameStart*40, 0, 40, 40, UnitPixel);
	}


	delete image;
}
