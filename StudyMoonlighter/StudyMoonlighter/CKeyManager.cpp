#include "pch.h"
#include "CKeyManager.h"

CKeyManager* CKeyManager::m_pInstance = nullptr;

CKeyManager::CKeyManager()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyManager::~CKeyManager()
{
}

bool CKeyManager::Key_Pressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}
// 이전에 눌림이 없고, 현재 눌렸을 경우
bool CKeyManager::Key_Down(KEYTYPE _type, int _iKey)
{
	if ((!m_bKeyState[_type][_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_type][_iKey] = !m_bKeyState[_type][_iKey];
		return true;
	}

	return false;
}

// 이전에 눌림이 있고, 현재 누르지 않을 경우
bool CKeyManager::Key_Up(KEYTYPE _type, int _iKey)
{
	if ((m_bKeyState[_type][_iKey]) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_type][_iKey] = !m_bKeyState[_type][_iKey];
		return true;
	}

	return false;
}

void CKeyManager::Update()
{
	for (int i = 0; i < KEY_END; ++i) {
		for (int j = 0; j < VK_MAX; ++j)
		{
			if ((!m_bKeyState[i][j]) && (GetAsyncKeyState(j) & 0x8000))
				m_bKeyState[i][j] = !m_bKeyState[i][j];

			if ((m_bKeyState[i][j]) && !(GetAsyncKeyState(j) & 0x8000))
				m_bKeyState[i][j] = !m_bKeyState[i][j];
		}
	}

}