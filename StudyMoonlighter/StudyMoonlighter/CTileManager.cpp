#include "pch.h"
#include "CTileManager.h"
#include "CAbstractFactory.h"
#include "CTile.h"
#include "CObject.h"
#include "CScrollManager.h"

CTileManager* CTileManager::m_pInstance = nullptr;

CTileManager::CTileManager()
{
	//m_vecTile.reserve(m_tileX * m_tileY);
}

void CTileManager::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(j * TILECX) + (TILECX >> 1) + 80;
			float fY = (float)(i * TILECY) + (TILECY >> 1) + 93;

			CObject* pTile = CAbstractFactory<CTile>::Create(fX, fY);
			m_vecTile.push_back(pTile);
		}
	}
}

int CTileManager::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
	return 0;
}

void CTileManager::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileManager::Render(HDC hDC)
{
	int		iScrollX = abs((int)CScrollManager::Get_Instance()->Get_ScrollX() / TILECX);
	int		iScrollY = abs((int)CScrollManager::Get_Instance()->Get_ScrollY() / TILECY);

	int		iMaxX = iScrollX + WINCX / TILECX + 2;
	int		iMaxY = iScrollY + WINCY / TILECY + 2;

	for (int i = iScrollY; i < iMaxY; ++i)
	{
		for (int j = iScrollX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

void CTileManager::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CObject*>);
	m_vecTile.clear();
}

void CTileManager::Picking_Tile(POINT pt)
{
	int		x = pt.x / TILECX;
	int		y = pt.y / TILECY;

	int	iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawIDPick();
}

void CTileManager::Save_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile/CTutorialTile1.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	bool		iDrawID(0);
	DWORD	dwByte(0);

	for (auto& pTile : m_vecTile)
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();

		WriteFile(hFile, pTile->Get_Info_Pointer(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &iDrawID, sizeof(bool), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Save", L"성공", MB_OK);
}

void CTileManager::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile/CTutorialTile1.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	bool		iDrawID(0);
	DWORD	dwByte(0);
	INFO	tTile{};

	Release();

	while (true)
	{
		bool a = ReadFile(hFile, &tTile, sizeof(INFO), &dwByte, NULL);
		bool b = ReadFile(hFile, &iDrawID, sizeof(bool), &dwByte, NULL);

		if (0 == dwByte)
			break;

		CObject* pTile = CAbstractFactory<CTile>::Create(tTile.fX, tTile.fY);
		static_cast<CTile*>(pTile)->Set_DrawID(iDrawID);

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Load", L"성공", MB_OK);
}
