#include "pch.h"
#include "CUiManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
CUiManager* CUiManager::m_pInstance = nullptr;

void CUiManager::Initialize()
{
	m_Inven = new CInventory();
	m_Inven->Initialize();
}

void CUiManager::Update()
{
	m_Inven->Update();
}

void CUiManager::Render(HDC hDC)
{
	if (m_eCurUi == UI_END) {
		SCENEID _id = CSceneManager::GetInstance()->Get_SceneID();
		switch (_id)
		{
		case SC_MENU:
			break;
		case SC_TUTORIAL:
		case SC_EDIT:
		case SC_GOLEMDUNGEON:
			m_eCurUi = UI_DUNGEON;
			break;
		case SC_VILLAGE:
			break;
		case SC_FIELD:
			break;
		case SC_GOLEMBOSS:
			break;
		case SC_ANIM:
			break;
		case SC_END:
			break;
		default:
			break;
		}
	}

	Image* image(nullptr);
	Graphics graphics(hDC);
	switch (m_eCurUi)
	{
	case UI_DUNGEON:
		Dungeon_Ui(hDC);
		break;
	case UI_INVEN:
		Inven_Ui(hDC);
		break;
	case UI_INVENCHEST:
		break;
	case UI_INVENSHOP:
		break;
	case UI_END:
		break;
	default:
		break;
	}

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 5, 10, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold1.png");
	graphics.DrawImage(image, 11, 16, 0, 0, 64, 64, UnitPixel);

	//hp ��
	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Circle.png");
	graphics.DrawImage(image, 80, 20, 0, 0, 48, 48, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Heart_Big.png");
	graphics.DrawImage(image, 88, 30, 0, 0, 32, 32, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Base.png");
	graphics.DrawImage(image, 115, 22, 0, 0, 146, 44, UnitPixel);

	int hpWidth =  (132 * CObjectManager::Get_Instance()->Get_Player()->Get_Hp()) / CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp();
	COLORREF color = RGB(237, 52, 52);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); 
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, 128, 32, 128+ hpWidth, 57, 10, 10);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont1 = CreateFont(
		25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szHpBar[64];
	_stprintf_s(szHpBar, _T("%d/%d"), CObjectManager::Get_Instance()->Get_Player()->Get_Hp(), CObjectManager::Get_Instance()->Get_Player()->Get_MaxHp());

	RECT rect = { 160, 50, 260, 70 }; 
	DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	TCHAR szMoney[64];
	_stprintf_s(szMoney, _T("%d"), static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_Money());

	RECT rect2 = { 20, 90, 70, 110 };
	DrawText(hDC, szMoney, _tcslen(szMoney), &rect2, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Coin.png");
	graphics.DrawImage(image, 10, 95, 0, 0, 18, 18, UnitPixel);

	delete image;
}

void CUiManager::Release()
{
	Safe_Delete<CInventory*>(m_Inven);
}

void CUiManager::Dungeon_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 920, 570, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_Pendant.png");
	graphics.DrawImage(image, 920, 570, 0, 0, 70, 70, UnitPixel);

	delete image;
}

void CUiManager::Village_Ui(HDC hDC)
{
}

void CUiManager::Inven_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Inventory_base.png");
	graphics.DrawImage(image, 100, 100, 0, 0, 1038, 548, UnitPixel);

	m_Inven->Render(hDC);
	delete image;
}
