#include "pch.h"
#include "CShopScene.h"
#include "CSoundManager.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CShopPlayer.h"
#include "CUiManager.h"
#include "CCollisionBox.h"
#include "CPortal.h"
#include "CCollisionManager.h"
#include "CShowCase.h"
#include "CShopTable.h"
#include "CShopSign.h"
#include "CCashRegister.h"
#include "CShopSellAll.h"
#include "CGamble.h"
#include "CInventoryManager.h"

#include "CVisitorIrish.h"
#include "CVisitorMerchantfemale.h"
#include "CVisitorMerchantmale.h"
#include "CVisitorOldfemale.h"

CShopScene::CShopScene():m_bShop(true), m_Active(false),offSetShopX(0),offSetShopY(0), m_bShopOpen(false), tick(0)
{
}

void CShopScene::Initialize()
{
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"shop_day_open.wav", g_fBackgroundVolume, true);
	
	int hp = CObjectManager::Get_Instance()->Get_Player()->Get_Hp();
	int money = static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_Money();
	int maxHp = CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp();
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PLAYER);
	CObjectManager::Get_Instance()->RenderListClear();
	CObjectManager::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CShopPlayer>::Create(564, 704));
	static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_State(hp, money, maxHp);

	CScrollManager::Get_Instance()->Set_Scroll(0, -260);
	m_fMapXSize = WINCX;
	m_fMapYSize = 1440;
	CScrollManager::Get_Instance()->Set_ScrollLock(m_fMapXSize, m_fMapYSize);

	Create_MapObj();
	CInventoryManager::GetInstance()->Get_InvenShop()->Set_Showcase();
	list<CObject*> mapObjList = CObjectManager::Get_Instance()->Get_MapObjList();

	m_showcaseItem.resize(4);
	m_showcasePrice.resize(4);
	m_CanNpcSpawn.resize(4);
	for (int i = 0; i < 4; ++i) {
		m_showcaseItem[i].itemId = ITEM_END;
		m_showcasePrice[i] = 0;
		m_CanNpcSpawn[i] = false;
	}
	CUiManager::GetInstance()->Set_UiType(UI_END);
}

int CShopScene::Update()
{
	m_showcaseItem = CInventoryManager::GetInstance()->Get_InvenShop()->Get_PriceSlotITEM();
	m_showcasePrice = CInventoryManager::GetInstance()->Get_InvenShop()->Get_PriceSlotPrice();

	tick++;
	if (m_bShopOpen) {
		if (tick > 500) {
			for (int i = 0; i < 4; ++i) {
				if ((m_showcaseItem[i].itemId != ITEM_END) && !m_CanNpcSpawn[i]) {
					switch (i)
					{
					case 0:
						CObjectManager::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory< CVisitorIrish>::Create(564, 720));
						static_cast<CVisitor*>(CObjectManager::Get_Instance()->Get_LastNPC())->Set_Init(0,564, 720, 430,530, m_showcaseItem[i], m_showcasePrice[i]);
						break;
					case 1:
						CObjectManager::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory< CVisitorMerchantfemale>::Create(564, 720));
						static_cast<CVisitor*>(CObjectManager::Get_Instance()->Get_LastNPC())->Set_Init(1, 564, 720, 430, 580, m_showcaseItem[i], m_showcasePrice[i]);
						break;
					case 2:
						CObjectManager::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory< CVisitorMerchantmale>::Create(564, 720));
						static_cast<CVisitor*>(CObjectManager::Get_Instance()->Get_LastNPC())->Set_Init(2, 564, 720, 430, 630, m_showcaseItem[i], m_showcasePrice[i]);
						break;
					case 3:
						CObjectManager::Get_Instance()->Add_Object(OBJ_NPC, CAbstractFactory< CVisitorOldfemale>::Create(564, 720));
						static_cast<CVisitor*>(CObjectManager::Get_Instance()->Get_LastNPC())->Set_Init(3, 564, 720, 430, 680, m_showcaseItem[i], m_showcasePrice[i]);
						break;
					default:
						break;
					}
					m_CanNpcSpawn[i] = true;
					CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
					CSoundManager::Get_Instance()->PlaySound(L"shop_visitor_enters_shop_bell.wav", SOUND_EFFECT, g_fEffectVolume, true);
					break;
				}
			}
			tick = 0;
		}
	}
	Key_Input();
	Offset();
	CObjectManager::Get_Instance()->Update();
	CUiManager::GetInstance()->Update();
	CInventoryManager::GetInstance()->Update();
	return 0;
}

void CShopScene::LateUpdate()
{
	m_Active = false;
	OnCollision();
	CheckOffset();
	CObjectManager::Get_Instance()->Late_Update();
	CUiManager::GetInstance()->Late_Update();
}

void CShopScene::Render(HDC hDC)
{
	COLORREF color = RGB(0, 0, 0);
	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, 0, 0, m_fMapXSize, m_fMapYSize);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	Image* image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/shop_background.png");
	Graphics graphics(hDC);
	graphics.DrawImage(image, WINCX/2-339 + iScrollX, iScrollY, 0, 0, 678, 868, UnitPixel);

	
	CObjectManager::Get_Instance()->Render(hDC);

	m_AddUiCheck = { 450 , 310, 650 , 510  };

	if (m_Active) {
		image = Image::FromFile(L"../MoonlighterAssets/Map/Shop/up_door.png");
		graphics.DrawImage(image, 513 + iScrollX, 360 + iScrollY, 0, 0, 108, 96, UnitPixel);
	}

	CUiManager::GetInstance()->Render(hDC);
	delete image;

	if (g_bDevmode) {
		if (m_Active) {
			Hitbox(hDC, m_AddUiCheck, iScrollX, iScrollY);
		}

	}
}

void CShopScene::Release()
{
	CObjectManager::Get_Instance()->Delete_ID(OBJ_MAPOBJ);
	CObjectManager::Get_Instance()->Delete_ID(OBJ_PORTAL);
	CObjectManager::Get_Instance()->RenderListClear();
}

void CShopScene::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_MODE,VK_F1)) {
		g_bDevmode = !g_bDevmode;
	}
}

void CShopScene::Create_MapObj()
{
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(WINCX/2, 760, 500, 10));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(280, 600, 10, 300));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(750, 600, 10, 300));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(415, 450, 250, 10));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(670, 450, 150, 10));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(540, 390, 10, 120));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(600, 390, 10, 120));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(670, 340, 130, 10));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(470, 340, 130, 10));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(400, 230, 10, 250));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(750, 230, 10, 250));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCollisionBox>::Create(570, 80, 350, 10));

	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShowCase>::Create(380,530));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShowCase>::Create(380, 580));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShowCase>::Create(380, 630));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShowCase>::Create(380, 680));


	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShopTable>::Create(630, 580));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CCashRegister>::Create(570, 580));

	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShopSign>::Create(500, 150));
	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CShopSellAll>::Create(450, 270));

	CObjectManager::Get_Instance()->Add_Object(OBJ_MAPOBJ, CAbstractFactory<CGamble>::Create(650, 150));

	CObjectManager::Get_Instance()->Add_Object(OBJ_PORTAL, CAbstractFactory<CPortal>::Create(565, 760, 80, 40));
	static_cast<CPortal*>(CObjectManager::Get_Instance()->Get_LastPortal())->Set_PortalType(VILLAGE);
}

void CShopScene::Offset()
{
	if (m_bShop) {
		if (CScrollManager::Get_Instance()->Get_ScrollY() != -260) {
			CScrollManager::Get_Instance()->Set_ScrollY(-5);
		}
	}
	else {
		if (CScrollManager::Get_Instance()->Get_ScrollY() != 100) {
			CScrollManager::Get_Instance()->Set_ScrollY(5);
		}
	}
}

void CShopScene::CheckOffset()
{
	if (CObjectManager::Get_Instance()->Get_Player()->Get_Info().fY < 400) {
		m_bShop = false;
	}
	else {
		m_bShop = true;
	}
}

void CShopScene::OnCollision()
{
	RECT rc{};
	if (IntersectRect(&rc, &m_AddUiCheck, CObjectManager::Get_Instance()->Get_Player()->Get_Rect()))
	{
		m_Active = true;
	}
}
