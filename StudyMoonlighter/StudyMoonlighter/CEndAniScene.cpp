#include "pch.h"
#include "CEndAniScene.h"
#include "CBitManager.h"
#include "CSoundManager.h"

CEndAniScene::CEndAniScene():m_Finish(false)
{
}

void CEndAniScene::Initialize()
{
	ADD_BMP(L"../MoonlighterAssets/StartAnim/final.bmp", L"Final");
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 23;
	m_tFrame.dwSpeed = 230;
	m_tFrame.dwTime = GetTickCount64();
	CSoundManager::Get_Instance()->StopAll();
	CSoundManager::Get_Instance()->PlayBGM(L"rynoka_night.wav", g_fBackgroundVolume + 0.1f, true);
}

int CEndAniScene::Update()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd) {
		m_Finish = true;
	}
	return 0;
}

void CEndAniScene::LateUpdate()
{
	if (!m_Finish) {
		if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
		{
			++m_tFrame.iFrameStart;

			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
				m_tFrame.iFrameStart = 0;

			m_tFrame.dwTime = GetTickCount64();
		}
	}
}

void CEndAniScene::Render(HDC hDC)
{
	HDC hMemDC = CBitManager::GetInstance()->FindImage(L"Final");
	GdiTransparentBlt(hDC, 0, 0, 1024, 720, hMemDC, m_tFrame.iFrameStart * 1024, 0, 1024, 720, RGB(255, 255, 255));

	COLORREF color = RGB(0, 0, 0);

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	Rectangle(hDC, 0, 620, 1030, 730);

	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	if (m_tFrame.iFrameStart < 14) {
		HFONT hFont1 = CreateFont(
			30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
		);
		HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

		TCHAR szHpBar[100];
		_stprintf_s(szHpBar, _T("윌은 돈을 가지고 마을로 돌아간다"));

		RECT rect = { WINCX / 2 - 500, 620, WINCX / 2 + 500, 730 };
		DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		SelectObject(hDC, OldFont);
		DeleteObject(hFont1);
	}
	else if(m_tFrame.iFrameStart < 21){
		HFONT hFont1 = CreateFont(
			30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"m3x6"
		);
		HFONT OldFont = (HFONT)SelectObject(hDC, hFont1);

		TCHAR szHpBar[100];
		_stprintf_s(szHpBar, _T("! 시청해주셔서 감사합니다 !"));

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
		_stprintf_s(szHpBar, _T("쥬신 151기 최유경"));

		RECT rect = { WINCX / 2 - 500, 620, WINCX / 2 + 500, 730 };
		DrawText(hDC, szHpBar, _tcslen(szHpBar), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		SelectObject(hDC, OldFont);
		DeleteObject(hFont1);
	}
}

void CEndAniScene::Release()
{
}

void CEndAniScene::Key_Input()
{
}

void CEndAniScene::Create_MapObj()
{
}

void CEndAniScene::Offset()
{
}
