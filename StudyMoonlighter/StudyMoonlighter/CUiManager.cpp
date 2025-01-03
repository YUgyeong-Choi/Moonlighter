#include "pch.h"
#include "CUiManager.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
CUiManager* CUiManager::m_pInstance = nullptr;

void CUiManager::Update()
{
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

	//hp ¹Ù
	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Circle.png");
	graphics.DrawImage(image, 80, 20, 0, 0, 48, 48, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Heart_Big.png");
	graphics.DrawImage(image, 88, 30, 0, 0, 32, 32, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Base.png");
	graphics.DrawImage(image, 115, 22, 0, 0, 146, 44, UnitPixel);

	int hpWidth =  (132 * CObjectManager::Get_Instance()->Get_Player()->Get_Hp()) / 100;
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

	delete image;
}

void CUiManager::Dungeon_Ui(HDC hDC)
{
	Image* image(nullptr);
	Graphics graphics(hDC);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Gold_circle.png");
	graphics.DrawImage(image, 920, 550, 0, 0, 70, 70, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/Bag_Pendant.png");
	graphics.DrawImage(image, 920, 550, 0, 0, 70, 70, UnitPixel);

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


	delete image;
}
