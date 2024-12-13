#pragma once
#include "Define.h"
#include "CObject.h"
class CTileManager
{
private:
	CTileManager();
	~CTileManager() { Release(); }
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

	void		Picking_Tile(POINT pt);

	void		Save_Tile();
	void		Load_Tile();

public:
	static CTileManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTileManager;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTileManager* m_pInstance;
	vector<CObject*>	m_vecTile;
};