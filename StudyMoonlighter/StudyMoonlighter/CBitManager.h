#pragma once
#include "CMyBmp.h"

class CBitManager
{
private:
	CBitManager();
	~CBitManager();

public:
	void		InsertBmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	void		InsertBmpList(const TCHAR* pFilePath, int num);
	HDC			FindImage(const TCHAR* pImgKey);
	void		Release();

public:
	static CBitManager* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CBitManager;

		return m_pInstance;
	}

	static void	DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CBitManager* m_pInstance;

	map<const TCHAR*, CMyBmp*>	m_mapBit;

};
