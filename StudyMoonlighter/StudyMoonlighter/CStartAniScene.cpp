#include "pch.h"
#include "CStartAniScene.h"
#include "CBitManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"
#include "CUiManager.h"
#include "CObjectManager.h"
#include "CAbstractFactory.h"
#include "CPlayer.h"

CStartAniScene::CStartAniScene():index(0), m_eCurType(STARTEND), m_ePreType(STARTEND), alpha(0), m_paid(false)
{
}

void CStartAniScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/StartAnim/house.bmp", L"House");
	ADD_BMP(L"../MoonlighterAssets/StartAnim/table.bmp", L"Table");
	m_eCurType = HOUSE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 33;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();
	CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"intro.wav", g_fBackgroundVolume+0.1f, true);
}

int CStartAniScene::Update()
{
	if (m_eCurType == HOUSE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
		m_paid = true;
	}
	if (m_paid) {
		alpha+=5;
	}

	if (alpha == 255) {
		alpha = 0;
		m_paid = false;
		m_eCurType = TABLE;
		m_tFrame.iFrameStart = 0;
		m_tFrame.iFrameEnd = 27;
		m_tFrame.dwSpeed = 250;
		m_tFrame.dwTime = GetTickCount64();
	}

	if (m_eCurType == TABLE && m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
		CSceneManager::GetInstance()->SetScene(SC_TUTORIAL);
	}

	return 0;
}

void CStartAniScene::LateUpdate()
{
	if (!m_paid) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;

			m_tFrame.dwTime = GetTickCount64();
		}
	}

}

void CStartAniScene::Render(HDC hDC)
{
	if (m_eCurType == HOUSE) {
		HDC hMemDC = CBitManager::GetInstance()->FindImage(L"House");
		GdiTransparentBlt(hDC, 0, 0, 1024, 720, hMemDC, m_tFrame.iFrameStart*1024, 0, 1024, 720, RGB(255, 255, 255));
	}
	else {
		HDC hMemDC = CBitManager::GetInstance()->FindImage(L"Table");
		GdiTransparentBlt(hDC, 0, 0, 1024, 720, hMemDC, m_tFrame.iFrameStart * 1024, 0, 1024, 720, RGB(255, 255, 255));
	}

	COLORREF color = RGB(0, 0, 0);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, 0, 620,1030, 730);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);


	if (m_eCurType == HOUSE) {
		HFONT hFont1 = CreateFont(
			30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
		);
		HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

		TCHAR szHpBar[100];
		_stprintf_s(szHpBar, _T("문라이터라는 오래된 상점을 운영하는 윌은"));

		RECT rect = { WINCX/2-300, 620, WINCX / 2 + 300, 730 };
		DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		SelectObject(hDC, OldFont);
		DeleteObject(hFont1);
	}
	else {
		if (m_tFrame.iFrameStart < 17) {
			HFONT hFont1 = CreateFont(
				30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
			);
			HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

			TCHAR szHpBar[100];
			_stprintf_s(szHpBar, _T("던전에서 전리품을 얻고 상점에 팔아 돈을 모을 계획을 세운다"));

			RECT rect = { WINCX / 2 - 500, 620, WINCX / 2 + 500, 730 };
			DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			SelectObject(hDC, OldFont);
			DeleteObject(hFont1);
		}
		else {
			HFONT hFont1 = CreateFont(
				30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
			);
			HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

			TCHAR szHpBar[100];
			_stprintf_s(szHpBar, _T("그렇게 윌은 던전으로 나서는데"));

			RECT rect = { WINCX / 2 - 500, 620, WINCX / 2 + 500, 730 };
			DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			SelectObject(hDC, OldFont);
			DeleteObject(hFont1);
		}
	}

	if (m_paid) {
		Image* image(nullptr);
		Graphics graphics(hDC);

		image = Image::FromFile(L"../MoonlighterAssets/Back/Back.png");
		ImageAttributes imgAttr;
		ColorMatrix cm = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha / 255.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		imgAttr.SetColorMatrix(&cm);
		graphics.DrawImage(image, Gdiplus::Rect(0, 0, 1024, 720), 0, 0, 1024, 720, UnitPixel, &imgAttr);
		delete image;
	}

}

void CStartAniScene::Release()
{
}

void CStartAniScene::Key_Input()
{
}

void CStartAniScene::Create_MapObj()
{
}

void CStartAniScene::Offset()
{
}
