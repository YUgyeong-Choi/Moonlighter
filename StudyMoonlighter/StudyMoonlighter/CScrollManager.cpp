#include "pch.h"
#include "CScrollManager.h"

CScrollManager* CScrollManager::m_pInstance = nullptr;

CScrollManager::CScrollManager() : m_fScrollX(0.f), m_fScrollY(0.f), m_fMapXSize(0.f), m_fMapYSize(0.f)
{
}

CScrollManager::~CScrollManager()
{
}

void CScrollManager::Scroll_Lock()
{
	if (0.f < m_fScrollX)
		m_fScrollX = 0.f;

	if (WINCX - m_fMapXSize > m_fScrollX)
		m_fScrollX = WINCX - m_fMapXSize;

	if (0.f < m_fScrollY)
		m_fScrollY = 0.f;

	if (WINCY - m_fMapYSize > m_fScrollY)
		m_fScrollY = WINCY - m_fMapYSize;
}
