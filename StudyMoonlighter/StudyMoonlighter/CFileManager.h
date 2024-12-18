#pragma once
#include "Define.h"
class CFileManager
{
public:
	CFileManager() { ; }
	~CFileManager() { ; }
public:
	static void Save_MapObj(const TCHAR* _pFilePath);
	static void Save_MonsterObj(const TCHAR* _pFilePath);
	static void Load_MapObjFile(const TCHAR* _pFilePath);
	static void Load_MonsterFile(const TCHAR* _pFilePath);
public:
	static CFileManager* GetInstance()
	{
		if (nullptr == m_pInstance) {
			m_pInstance = new CFileManager;
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
	static CFileManager* m_pInstance;
};