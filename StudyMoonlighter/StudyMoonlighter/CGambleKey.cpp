#include "pch.h"
#include "CGambleKey.h"
#include "CBitManager.h"
#include "CKeyManager.h"
#include "CUiManager.h"
#include "CSoundManager.h"
#include "CObjectManager.h"
#include "CShopPlayer.h"

CGambleKey::CGambleKey():m_bActive(false),y(0), index(0), m_first(false),m_second(false),m_third(false), m_firstIndex(0),m_secondIndex(0),m_thirdIndex(0), m_finish(false), m_Win(false), m_goldDown(0)
{
}

void CGambleKey::Initialize()
{
	m_firstFruit.push_back(CHERRY);
	m_firstFruit.push_back(LEMON);
	m_firstFruit.push_back(STRAW);
	m_firstFruit.push_back(GRAPE);
	m_firstFruit.push_back(WATER);
	m_firstFruit.push_back(BANANA);
	m_firstFruit.push_back(ORANGE);

	m_secondFruit.push_back(ORANGE);
	m_secondFruit.push_back(CHERRY);
	m_secondFruit.push_back(BANANA);
	m_secondFruit.push_back(STRAW);
	m_secondFruit.push_back(WATER);
	m_secondFruit.push_back(LEMON);
	m_secondFruit.push_back(GRAPE);

	m_thridFruit.push_back(GRAPE);
	m_thridFruit.push_back(CHERRY);
	m_thridFruit.push_back(BANANA);
	m_thridFruit.push_back(ORANGE);
	m_thridFruit.push_back(WATER);
	m_thridFruit.push_back(STRAW);
	m_thridFruit.push_back(LEMON);
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();
}

int CGambleKey::Update()
{
	KeyInput();
	if (m_bActive) {
		y+=5;
		if (y > 75) {
			y = 0;
			index++;
			if (index > 6) {
				index = 0;
			}
		}
	}
	return 0;
}

void CGambleKey::Late_Update()
{
	if (m_Win) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = 0;
				m_goldDown += 5;
			}
				

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

void CGambleKey::Render(HDC hDC)
{

	Graphics graphics(hDC);

	if (m_first) {
		Image* image = nullptr;
		image = LoadImageForFruit(m_firstFruit[m_firstIndex]);
		graphics.DrawImage(image, 397, 282, 0, 0, 69, 75, UnitPixel);
		delete image;
	}
	else {
		Image* image = nullptr;
		Image* image2 = nullptr;
		image = LoadImageForFruit(m_firstFruit[index]);

		if (index == 6) {
			image2 = LoadImageForFruit(CGambleKey::CHERRY);
		}
		else {
			image2 = LoadImageForFruit(m_firstFruit[index + 1]);
		}

		graphics.DrawImage(image, 397, 282, 0, 0 - y, 69, 75, UnitPixel);
		graphics.DrawImage(image2, 397, 282, 0, 75 - y, 69, 75, UnitPixel);
		delete image;
		delete image2;
	}

	if (m_second) {
		Image* image = nullptr;
		image = LoadImageForFruit(m_secondFruit[m_secondIndex]);
		graphics.DrawImage(image, 480, 282, 0, 0, 69, 75, UnitPixel);
		delete image;
	}
	else {
		Image* image3 = nullptr;
		Image* image4 = nullptr;
		image3 = LoadImageForFruit(m_secondFruit[index]);

		if (index == 6) {
			image4 = LoadImageForFruit(CGambleKey::ORANGE);
		}
		else {
			image4 = LoadImageForFruit(m_secondFruit[index + 1]);
		}

		graphics.DrawImage(image3, 480, 282, 0, 0 - y, 69, 75, UnitPixel);
		graphics.DrawImage(image4, 480, 282, 0, 75 - y, 69, 75, UnitPixel);
		delete image3;
		delete image4;
	}

	if (m_third) {
		Image* image = nullptr;
		image = LoadImageForFruit(m_thridFruit[m_thirdIndex]);
		graphics.DrawImage(image, 555, 282, 0, 0, 69, 75, UnitPixel);
		delete image;
	}
	else {
		Image* image5 = nullptr;
		Image* image6 = nullptr;
		image5 = LoadImageForFruit(m_thridFruit[index]);

		if (index == 6) {
			image6 = LoadImageForFruit(CGambleKey::GRAPE);
		}
		else {
			image6 = LoadImageForFruit(m_thridFruit[index + 1]);
		}

		graphics.DrawImage(image5, 555, 282, 0, 0 - y, 69, 75, UnitPixel);
		graphics.DrawImage(image6, 555, 282, 0, 75 - y, 69, 75, UnitPixel);
		delete image5;
		delete image6;
	}

	if (m_Win) {
		HDC hMemDC = CBitManager::GetInstance()->FindImage(L"GoldFalling");
		GdiTransparentBlt(hDC, 100, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 200, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 300, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 400, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 500, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 600, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 700, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
		GdiTransparentBlt(hDC, 800, 0 + ((m_tFrame.iFrameStart + m_goldDown) * 30), 64, 64, hMemDC, 0, 64 * m_tFrame.iFrameStart, 64, 64, RGB(255, 255, 255));
	}

}

void CGambleKey::Release()
{
}

void CGambleKey::KeyInput()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_GAMBLE, VK_SPACE) && (CUiManager::GetInstance()->Get_UiType()== UI_GAMBLE)) {
		if (m_first && m_second && !m_third) {
			m_third = true;
			if (y < 35) {
				m_thirdIndex = index;
			}
			else {
				if (index == 6) {
					m_thirdIndex = 0;
				}
				else {
					m_thirdIndex = index + 1;
				}
			}
			m_finish = true;
			CSoundManager::Get_Instance()->StopAll();
			CheckWin();
		}

		if (m_first && !m_second) {
			m_second = true;
			if (y < 35) {
				m_secondIndex = index;
			}
			else {
				if (index==6) {
					m_secondIndex = 0;
				}
				else {
					m_secondIndex = index + 1;
				}	
			}
		}


		if (m_first == false) {
			m_first = true;
			if (y < 35) {
				m_firstIndex = index;
			}
			else {
				if (index == 6) {
					m_firstIndex = 0;
				}
				else {
					m_firstIndex = index + 1;
				}
			}
			
		}
	}

	if (CKeyManager::Get_Instance()->Key_Down(KEY_GAMBLE, 'R')) {
		if (m_first && m_second && m_third) {
			m_third = false;
			m_second = false;
			m_first = false;
			m_Win = false;
			m_finish = false;
			m_goldDown = 0;
			y = 0;
			index = 0;
			CSoundManager::Get_Instance()->StopAll();
			CSoundManager::Get_Instance()->PlayBGM(L"slotmove.wav", g_fBackgroundVolume, true);
		}
	}
}

void CGambleKey::CheckWin()
{
	if ((m_firstFruit[m_firstIndex] == m_secondFruit[m_secondIndex]) && (m_secondFruit[m_secondIndex] == m_thridFruit[m_thirdIndex])) {
		CSoundManager::Get_Instance()->PlaySound(L"jackpot.wav", SOUND_EFFECT, g_fEffectVolume, false);
		m_Win = true;
		int money = static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_Money();
		static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Money(money);
	}
	else {
		CSoundManager::Get_Instance()->PlaySound(L"fail.wav", SOUND_EFFECT, g_fEffectVolume, false);
		int money = static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Get_Money();
		static_cast<CShopPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Money(-money);
		m_Win = false;
	}
}

void CGambleKey::InitGame()
{
	m_third = false;
	m_second = false;
	m_first = false;
	m_Win = false;
	m_finish = false;
	m_goldDown = 0;
	y = 0;
	index = 0;
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"slotmove.wav", g_fBackgroundVolume, true);
}

Image* CGambleKey::LoadImageForFruit(FRUITTYPE fruitType)
{
	switch (fruitType) {
	case CGambleKey::CHERRY: return Image::FromFile(L"../MoonlighterAssets/Gamble/cherry.png");
	case CGambleKey::LEMON: return Image::FromFile(L"../MoonlighterAssets/Gamble/lemon.png");
	case CGambleKey::STRAW: return Image::FromFile(L"../MoonlighterAssets/Gamble/strawberry.png");
	case CGambleKey::GRAPE: return Image::FromFile(L"../MoonlighterAssets/Gamble/grape.png");
	case CGambleKey::WATER: return Image::FromFile(L"../MoonlighterAssets/Gamble/watermelon.png");
	case CGambleKey::BANANA: return Image::FromFile(L"../MoonlighterAssets/Gamble/banana.png");
	case CGambleKey::ORANGE: return Image::FromFile(L"../MoonlighterAssets/Gamble/orange.png");
	default: return nullptr;
	}
}

