#include "pch.h"
#include "CPlayer.h"
#include "CBitManager.h"

CPlayer::CPlayer():m_bIsRoll(false), m_eCurState(END), m_ePreState(END)
{
}

void CPlayer::Initialize()
{
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Right/will_walking_cycle_right.bmp", L"Will_Walking_right");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Left/will_walking_cycle_left.bmp", L"Will_Walking_left");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Up/will_walking_cycle_up.bmp", L"Will_Walking_up");
	CBitManager::GetInstance()->InsertBmp(L"../MoonlighterAssets/Player/Walk/Down/will_walking_cycle_down.bmp", L"Will_Walking_down");

	m_pImgKey = L"Will_Walking_down";
	m_eCurState = IDLE;
	m_ePreState = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount64();
}

int CPlayer::Update()
{
	Key_Input();
	Change_Motion();
	return 0;
}

void CPlayer::Late_Update()
{
	Offset();
	__super::Move_Frame();
}

void CPlayer::Render(HDC hDC)
{
	HDC		hMemDC = CBitManager::GetInstance()->FindImage(m_pImgKey);
	GdiTransparentBlt(hDC, 10 ,10, 23, 40, hMemDC,23 * m_tFrame.iFrameStart, 0, 23, 40, RGB(255, 255, 255));		// 제거할 색상
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
}

void CPlayer::Offset()
{
}

void CPlayer::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}
}
