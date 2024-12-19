#pragma once

class CUiManager
{
public:
	CUiManager() { ; }
	~CUiManager() { ; }
public:
	static CUiManager* GetInstance()
	{
		if (nullptr == m_pInstance) {
			m_pInstance = new CUiManager;
		}

		return m_pInstance;
	}

	static void DestroyInstance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static CUiManager* m_pInstance;
};