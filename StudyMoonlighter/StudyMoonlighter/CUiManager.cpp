#include "pch.h"
#include "CUiManager.h"
#include "CObjectManager.h"
CUiManager* CUiManager::m_pInstance = nullptr;

void CUiManager::Render(HDC hDC)
{
	Image* image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Circle.png");
	Graphics graphics(hDC);
	graphics.DrawImage(image, 80, 30, 0, 0, 48, 48, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Heart_Big.png");
	graphics.DrawImage(image, 88, 40, 0, 0, 32, 32, UnitPixel);

	image = Image::FromFile(L"../MoonlighterAssets/Ui/HealthBar_Base.png");
	graphics.DrawImage(image, 115, 32, 0, 0, 146, 44, UnitPixel);

	int hpWidth =  (132 * CObjectManager::Get_Instance()->Get_Player()->Get_Hp()) / 100;
	COLORREF color = RGB(237, 52, 52);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); 
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	RoundRect(hDC, 128, 42, 128+ hpWidth, 67, 10, 10);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	delete image;
}
