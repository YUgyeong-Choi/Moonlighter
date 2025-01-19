#include "pch.h"
#include "CPriceSlot.h"
#include "CKeyManager.h"
#include "CUiManager.h"
#include "CSoundManager.h"
#include "CInventoryManager.h"

CPriceSlot::CPriceSlot() : m_iPrice(0), key_type(KEY_END), m_iPriceIndex(0), m_bActive(false)
{
	item.itemId = ITEM_END;
	item.num = 0;
	m_OutfitType = OUTFIT_ITEM;
	for (int i = 0; i < 6; ++i) {
		m_OnePrice[i] = 0;
	}
}

CPriceSlot::CPriceSlot(int _row, int _column, int num) : m_iPrice(0), key_type(KEY_END), m_iPriceIndex(0), m_bActive(false)
{
	rowIndex = _row, columnIndex = _column;
	item.itemId = ITEM_END;
	item.num = 0;
	for (int i = 0; i < 6; ++i) {
		m_OnePrice[i] = 0;
	}
	m_iPriceIndex = 5;
	switch (num)
	{
	case 0:
		key_type = KEY_PRICE0;
		break;
	case 1:
		key_type = KEY_PRICE1;
		break;
	case 2:
		key_type = KEY_PRICE2;
		break;
	case 3:
		key_type = KEY_PRICE3;
		break;
	default:
		break;
	}
}

void CPriceSlot::Initialize()
{
}

int CPriceSlot::Update()
{
	Key_Input();
	Calc_Price();
	return 0;
}

void CPriceSlot::Late_Update()
{
}

void CPriceSlot::Render(HDC hDC)
{
	SetTextColor(hDC, RGB(0, 0, 0));
	SetBkMode(hDC, TRANSPARENT);

	HFONT hFont1 = CreateFont(
		22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m5x7"
	);

	HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

	TCHAR szPrice[64];
	_stprintf_s(szPrice, _T("%d%d%d%d%d%d"), m_OnePrice[0], m_OnePrice[1], m_OnePrice[2], m_OnePrice[3], m_OnePrice[4], m_OnePrice[5]);

	RECT rect = { 530 + (columnIndex * 200) , 195 + ((rowIndex-1) * 100), 680 + (columnIndex  * 200), 255 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szPrice, _tcslen(szPrice), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SetTextColor(hDC, RGB(20, 157, 114));
	TCHAR szCount[64];
	_stprintf_s(szCount, _T("x%d"), item.num);

	RECT rect2 = { 640 + (columnIndex * 200) , 230 + ((rowIndex - 1) * 100), 680 + (columnIndex * 200), 250 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szCount, _tcslen(szCount), &rect2, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	SetTextColor(hDC, RGB(236, 221, 192));
	TCHAR szPriceText[64];
	_stprintf_s(szPriceText, _T("รัพื:"));
	RECT rect3 = { 530 + (columnIndex * 200) , 250 + ((rowIndex - 1) * 100), 580 + (columnIndex * 200), 270 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szPriceText, _tcslen(szPriceText), &rect3, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	TCHAR szTotalPrice[64];
	_stprintf_s(szTotalPrice, _T("%d"), item.num * m_iPrice);
	RECT rect4 = { 600 + (columnIndex * 200) , 250 + ((rowIndex - 1) * 100), 670 + (columnIndex * 200), 270 + ((rowIndex - 1) * 100) };
	DrawText(hDC, szTotalPrice, _tcslen(szTotalPrice), &rect4, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	SelectObject(hDC, OldFont);
	DeleteObject(hFont1);

	if (m_bActive) {
		Image* image(nullptr);
		Graphics graphics(hDC);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/Showcase_arrow_up.png");
		graphics.DrawImage(image, 572 + (m_iPriceIndex*11) + (columnIndex * 200), 210 + ((rowIndex - 1) * 100), 0, 0, 11, 8, UnitPixel);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/Showcase_arrow_down.png");
		graphics.DrawImage(image, 572 + (m_iPriceIndex * 11) + (columnIndex * 200), 235 + ((rowIndex - 1) * 100), 0, 0, 11, 8, UnitPixel);
		delete image;
	}


	if (g_bDevmode) {
		DetectRect(hDC, rect,0,0);
		DetectRect(hDC, rect2,0,0);
		DetectRect(hDC, rect3,0,0);
		DetectRect(hDC, rect4,0,0);
	}
}

void CPriceSlot::Release()
{
}

void CPriceSlot::Init_Price()
{
	for (int i = 0; i < 6; ++i) {
		m_OnePrice[i] = 0;
	}
}

void CPriceSlot::Calc_Price()
{
	m_iPrice = 0;
	int multipliers[] = { 100000, 10000, 1000, 100, 10, 1 };

	for (int i = 0; i < 6; ++i) {
		m_iPrice += m_OnePrice[i] * multipliers[i];
	}
}

void CPriceSlot::Key_Input()
{
	if (CKeyManager::Get_Instance()->Key_Down(key_type, 'A') && m_bActive) {
		if (CInventoryManager::GetInstance()->Get_InvenShop()->Get_IsPriceTime()) {
			m_iPriceIndex--;
			if (m_iPriceIndex < 0) {
				m_iPriceIndex = 0;
			}
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(key_type, 'D') && m_bActive) {
		if (CInventoryManager::GetInstance()->Get_InvenShop()->Get_IsPriceTime()) {
			m_iPriceIndex++;
			if (m_iPriceIndex > 5) {
				m_iPriceIndex = 5;
			}
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(key_type, 'W') && m_bActive) {
		if (CInventoryManager::GetInstance()->Get_InvenShop()->Get_IsPriceTime()) {
			m_OnePrice[m_iPriceIndex]++;
			if (m_OnePrice[m_iPriceIndex] > 9) {
				m_OnePrice[m_iPriceIndex] = 9;
			}
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(key_type, 'S') && m_bActive) {
		if (CInventoryManager::GetInstance()->Get_InvenShop()->Get_IsPriceTime()) {
			m_OnePrice[m_iPriceIndex]--;
			if (m_OnePrice[m_iPriceIndex] < 0) {
				m_OnePrice[m_iPriceIndex] = 0;
			}
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}


	if (CKeyManager::Get_Instance()->Key_Down(key_type , 'K') && m_bActive) {
		if (CInventoryManager::GetInstance()->Get_InvenShop()->Get_IsPriceTime()) {
			CInventoryManager::GetInstance()->Get_InvenShop()->Off_IsPriceTime();
			m_bActive = false;
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_selector_movement.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}
}
