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

void CBitManager::InsertBmpList(const TCHAR* pFilePath, int num)
{
	for (int i = 0; i < num; ++i) {
		wstring bmpFile = pFilePath + to_wstring(i + 1) + L".bmp";
		CBitManager::GetInstance()->InsertBmp(bmpFile.c_str(), (L"Main_menu_1_" + to_wstring(i + 1)).c_str());
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