#include "pch.h"
#include "CItem.h"
#include "CScrollManager.h"
#include "CObjectManager.h"
#include "CCollisionManager.h"

CItem::CItem():m_type(ITEM_END), m_outfitType(OUTFIT_END)
{
}

void CItem::Initialize()
{
    m_eOBJID = OBJ_ITEM;
	m_eRender = RENDER_GAMEOBJECT;

	m_tInfo.fCX = 10;
	m_tInfo.fCY = 10;

	m_tRenderSizeX = 48;
	m_tRenderSizeY = 48;

	m_fSpeed = 1;
	m_iHp = 1;
}

int CItem::Update()
{
	if (m_iHp <= 0) {
		return OBJ_DEAD;
	}
	m_HitBox = { (int)m_tInfo.fX - 35, (int)m_tInfo.fY - 35,(int)m_tInfo.fX + 35, (int)m_tInfo.fY + 35 };
	__super::Update_Rect();
    return 0;
}

void CItem::Late_Update()
{
	OnCollision();
}

void CItem::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();

	Image* image(nullptr);
	Graphics graphics(hDC);
	image = Image::FromFile(pImageUrl);
	
	graphics.DrawImage(image, (int)m_tRenderRect.left+iScrollX, (int)m_tRenderRect.top +iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);

	delete image;

	if (g_bDevmode) {
		Renderbox(hDC, m_tRenderRect, iScrollX, iScrollY);
		Hitbox(hDC, m_tRect, iScrollX, iScrollY);
		DetectCircle(hDC, m_HitBox);
	}
}

void CItem::Release()
{
}

void CItem::OnCollision(CObject* _obj)
{
	if (_obj->Get_OBJID() == OBJ_PLAYER) {
		m_iHp--;
	}
}

void CItem::Set_ItemType(ITEMTYPE _type)
{
    m_type = _type;
	switch (_type)
	{
	case BROKENSWORD:
		pImageUrl = L"../MoonlighterAssets/Items/0_BrokenSword.png";
		break;
	case FABRIC:
		break;
	case HARDENEDSTEEL:
		pImageUrl = L"../MoonlighterAssets/Items/2_HardenedSteel.png";
		break;
	case IRONBAR:
		pImageUrl = L"../MoonlighterAssets/Items/3_IronBar.png";
		break;
	case RICHJELLY:
		pImageUrl = L"../MoonlighterAssets/Items/4_RichJelly.png";
		break;
	case TEETHSTONE:
		pImageUrl = L"../MoonlighterAssets/Items/5_TeethStone.png";
		break;
	case WATERSPHERE:
		pImageUrl = L"../MoonlighterAssets/Items/6_WaterSphere.png";
		break;
	case WHETSTONE:
		pImageUrl = L"../MoonlighterAssets/Items/7_Whetstone.png";
		break;
	case POTION:
		pImageUrl = L"../MoonlighterAssets/Items/potion.png";
		break;
	case ITEM_END:
		break;
	default:
		break;
	}
}

void CItem::DetectedPlayer()
{
}

void CItem::OnCollision() 
{
	CObject* _copyPlayer = CObjectManager::Get_Instance()->Get_Player();

	
	float fRadius = (_copyPlayer->Get_Info().fCX + 70) * 0.5f;

	float fWidth = abs(_copyPlayer->Get_Info().fX - m_tInfo.fX);
	float fHeight = abs(_copyPlayer->Get_Info().fY - m_tInfo.fY);

	float fDiagonal = powf((fWidth * fWidth + fHeight * fHeight), 0.5f);

	bool check =  fRadius >= fDiagonal;

	if (check) {
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f), m_fAngle(0.f);

		fWidth = _copyPlayer->Get_Info().fX - m_tInfo.fX;
		fHeight = _copyPlayer->Get_Info().fY - m_tInfo.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		fRadian = acosf(fWidth / fDiagonal);

		m_fAngle = fRadian * (180.f / PI);

		if (_copyPlayer->Get_Info().fY > m_tInfo.fY)
			m_fAngle *= -1.f;

		// degree to radian
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));
	}
}
 