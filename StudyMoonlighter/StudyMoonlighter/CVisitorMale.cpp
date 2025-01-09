#include "pch.h"
#include "CVisitorMale.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CSoundManager.h"
#include "CSceneManager.h"
#include "CObjectManager.h"
#include "CPlayer.h"

CVisitorMale::CVisitorMale(): charIndex(0), tick(0), cnt(0), talkFinish(false), m_Appear(false), m_dead(false)
{
	_stprintf_s(text[0], _T("도박으로 전재산을 잃었다는 소문이"));
	_stprintf_s(text[1], _T("마을 전체에 퍼졌네"));
	_stprintf_s(text[2], _T("아이고,,,"));
	_stprintf_s(text[3], _T("골렘 보스를 잡으면"));
	_stprintf_s(text[4], _T("돈을 많이 준다는 소문이 있네"));
	_stprintf_s(text[5], _T("도전해 보거라"));
	_stprintf_s(name, _T("Zenon"));
}

void CVisitorMale::Initialize()
{
	m_eOBJID = OBJ_NPC;

	m_tInfo.fCX = 15.f;
	m_tInfo.fCY = 15.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 7;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 100.f;
	m_tRenderSizeY = 100.f;
	m_eRender = RENDER_GAMEOBJECT;

	CSoundManager::Get_Instance()->PlaySound(L"slimychest_appear.wav", SOUND_EFFECT, g_fEffectVolume, true);
	static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Inven();
}

int CVisitorMale::Update()
{
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd && m_dead) {
		static_cast<CPlayer*>(CObjectManager::Get_Instance()->Get_Player())->Set_Inven();
		return OBJ_DEAD;
	}

	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd && !m_Appear) {
		m_Appear = true;
		m_tFrame.iFrameStart = 0;
	}

	if (m_Appear) {
		tick++;
	}
	

	if (tick > 6) {
		if (charIndex < _tcslen(text[cnt])) {
			charIndex++;
			tick = 0;
		}
		else {
			talkFinish = true;
		}
	}
	KeyInput();
	__super::Update_Rect();
	return 0;
}

void CVisitorMale::Late_Update()
{
	if (!m_Appear) {
		__super::Move_Frame();
	}
	if (m_dead) {
		__super::Move_Frame();
	}
}

void CVisitorMale::Render(HDC hDC)
{
	Image* image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_idle_right.png");
	Graphics graphics(hDC);
	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();
	if (m_Appear && !m_dead) {
		graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, 0, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);
		image = Image::FromFile(L"../MoonlighterAssets/Ui/talk.png");
		graphics.DrawImage(image, 170, 500, 0, 0, 600, 200, UnitPixel);

		image = Image::FromFile(L"../MoonlighterAssets/Ui/grandpa.png");
		graphics.DrawImage(image, 185, 520, 0, 0, 180, 170, UnitPixel);

		image = Image::FromFile(L"../MoonlighterAssets/Ui/button_K.png");
		graphics.DrawImage(image, 700, 630, 0, 0, 64, 64, UnitPixel);

		HFONT hFont = CreateFont(
			30,
			0,
			0,
			0,
			FW_BOLD,
			FALSE,
			FALSE,
			FALSE,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			NULL
		);
		HFONT OldFont = (HFONT)SelectObject(hDC, hFont);

		SetTextColor(hDC, RGB(62, 44, 8));
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, 400, 530, name, _tcslen(name));

		SetTextColor(hDC, RGB(130, 114, 86));
		SetBkMode(hDC, TRANSPARENT);

		TextOut(hDC, 400, 570, text[cnt], charIndex);

		DeleteObject(hFont);
		SelectObject(hDC, OldFont);

	}


	if (!m_Appear) {
		Image* image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/appear.png");
		Graphics graphics(hDC);

		graphics.DrawImage(image, (int)m_tInfo.fX-32 + iScrollX, (int)m_tInfo.fY - 32+ iScrollY, m_tFrame.iFrameStart*64, 0, (int)64, (int)64, UnitPixel);

		delete image;
	}

	if (m_dead) {
		Image* image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/appear.png");
		Graphics graphics(hDC);

		graphics.DrawImage(image, (int)m_tInfo.fX - 32 + iScrollX, (int)m_tInfo.fY - 32 + iScrollY, m_tFrame.iFrameStart * 64, 0, (int)64, (int)64, UnitPixel);

		delete image;
	}

	delete image;
}

void CVisitorMale::Release()
{
}


void CVisitorMale::KeyInput()
{
	if (CKeyManager::Get_Instance()->Key_Down(KEY_TALK, 'K')) {
		if (talkFinish) {
			cnt++;
			talkFinish = false;
			charIndex = 0;
			if (cnt == 6) {
				m_dead = true;
			}
			CSoundManager::Get_Instance()->StopSound(SOUND_EFFECT);
			CSoundManager::Get_Instance()->PlaySound(L"gui_press_button.wav", SOUND_EFFECT, g_fEffectVolume, true);
		}
	}
}
