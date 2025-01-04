#include "pch.h"
#include "CVisitorMale.h"
#include "CScrollManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

CVisitorMale::CVisitorMale():m_bXArrive(false), m_bYArrive(false), charIndex(0), tick(0), cnt(0), talkFinish(false)
{
	_stprintf_s(text[0], _T("튜토리얼을 하느라 수고했네,,,"));
	_stprintf_s(text[1], _T("이제 게임을 시작할 때가 되었구나"));
	_stprintf_s(text[2], _T("던전에서 전리품을 얻고"));
	_stprintf_s(text[3], _T("상점에서 물건을 팔아"));
	_stprintf_s(text[4], _T("** 부자 ** 가 되어라"));
	_stprintf_s(text[5], _T("행운을 빌겠네"));
	_stprintf_s(name, _T("Zenon"));
}

void CVisitorMale::Initialize()
{
	m_eOBJID = OBJ_NPC;

	m_tInfo.fCX = 15.f;
	m_tInfo.fCY = 15.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 10;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tRenderSizeX = 100.f;
	m_tRenderSizeY = 100.f;
	m_eRender = RENDER_GAMEOBJECT;

	m_fSpeed = 1.f;
	m_eCurPattern = CVisitor::WALK;

	m_eDir = UP;
}

int CVisitorMale::Update()
{
	tick++;

	if (tick > 6) {
		if (m_bYArrive && m_bXArrive) {
			if (charIndex < _tcslen(text[cnt])) {
				charIndex++;
				tick = 0;
			}
			else {
				talkFinish = true;
			}
		}

	}
	if (m_bYArrive) {
		m_eDir = LEFT;
	}
	TarGetPosition();
	Change_Motion();
	KeyInput();
	__super::Update_Rect();
	return 0;
}

void CVisitorMale::Late_Update()
{
	if (m_eCurPattern == CVisitor::WALK) {
		__super::Move_Frame();
	}
}

void CVisitorMale::Render(HDC hDC)
{
	Image* image = Image::FromFile(L"MoonlighterAssets/Visitor/Male/male_idle_right");
	Graphics graphics(hDC);

	int		iScrollX = (int)CScrollManager::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollManager::Get_Instance()->Get_ScrollY();


	switch (m_eCurPattern)
	{
	case CVisitor::IDLE:
		switch (m_eDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_idle_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_idle_right.png");
			break;
		case UP:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_idle_up.png");
			break;
		case DOWN:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_idle_down.png");
			break;
		}
		break;
	case CVisitor::WALK:
		switch (m_eDir)
		{
		case LEFT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_walk_left.png");
			break;
		case RIGHT:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_walk_right.png");
			break;
		case UP:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_walk_up.png");
			break;
		case DOWN:
			image = Image::FromFile(L"../MoonlighterAssets/Visitor/Male/male_walk_down.png");
			break;
		}
		break;
	}
	graphics.DrawImage(image, (int)m_tRenderRect.left + iScrollX, (int)m_tRenderRect.top + iScrollY, (int)m_tRenderSizeX * m_tFrame.iFrameStart, 0, (int)m_tRenderSizeX, (int)m_tRenderSizeY, UnitPixel);


	if (m_bXArrive && m_bYArrive) {
		image = Image::FromFile(L"../MoonlighterAssets/Ui/talk.png");
		graphics.DrawImage(image, 170, 500, 0 , 0, 600, 200, UnitPixel);

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
	delete image;
}

void CVisitorMale::Release()
{
}

void CVisitorMale::TarGetPosition()
{
	if (1000 < m_tInfo.fY) {
		m_tInfo.fY -= 1;
	}
	else {
		m_bYArrive = true;
	}

	if (m_bYArrive) {
		if (1100 < m_tInfo.fX) {
			m_tInfo.fX -= 1;
		}
		else {
			m_bXArrive = true;
			m_eCurPattern = CVisitor::IDLE;
		}
	}
}

void CVisitorMale::Change_Motion()
{
	if (m_ePrePattern != m_eCurPattern) {
		switch (m_eCurPattern)
		{
		case CVisitor::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CVisitor::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;
		case CVisitor::END:
			break;
		default:
			break;
		}
		m_ePrePattern = m_eCurPattern;
	}
}

void CVisitorMale::KeyInput()
{
	if (CKeyManager::Get_Instance()->Key_Down('K')) {
		if (talkFinish) {
			cnt++;
			talkFinish = false;
			charIndex = 0;
			if (cnt == 5) {
				CSceneManager::GetInstance()->SetScene(SC_VILLAGE);
			}
		}
	}
}
