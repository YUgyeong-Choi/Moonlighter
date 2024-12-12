#include "pch.h"
#include "CBitManager.h"
CBitManager* CBitManager::m_pInstance = nullptr;

CBitManager::CBitManager()
{
}

CBitManager::~CBitManager()
{
	Release();
}

void CBitManager::InsertBmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp* pMyBmp = new CMyBmp;
		pMyBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pMyBmp });
	}
}

HDC CBitManager::FindImage(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

void CBitManager::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), DeleteMap());
	m_mapBit.clear();
}