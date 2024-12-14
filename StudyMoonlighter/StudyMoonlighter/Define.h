#pragma once
#define WINCX		1280
#define WINCY		720

#define	PURE		= 0

#define VK_MAX				0xff

#define TILECX				32
#define TILECY				32

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1	

#define ADD_BMP(file, name)   CBitManager::GetInstance()->InsertBmp(file, name); 

extern HWND		g_hWnd;
extern bool		g_bDevmode;

enum SCENEID { SC_MENU, SC_VILLAGE, SC_END };
enum OBJID {OBJ_PLAYER, OBJ_MAPOBJ, OBJ_END};
enum RENDERID { RENDER_BACKGROUND, RENDER_GAMEOBJECT, RENDER_UI, RENDER_END };

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

struct tagFinder
{
public:
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& rObj)
	{
		return !lstrcmp(m_pString, rObj.first);
	}

private:
	const TCHAR* m_pString;
};

struct DeleteMap
{
public:
	template<typename T>
	void	operator()(T& Pair)
	{
		if (Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	ULONGLONG	dwSpeed;
	ULONGLONG	dwTime;

}FRAME;

typedef struct tagInfo
{
	float		fX, fY;
	float		fCX, fCY;	// 가로, 세로 길이

}INFO;


static BOOL Hitbox(HDC hDC, RECT tRect, float x, float y)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);
	Rectangle(hDC, (int)tRect.left + x, (int)tRect.top +y, (int)tRect.right + x, (int)tRect.bottom + y);
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);

	DeleteObject(hNewPen);
	return true;
}

static BOOL Renderbox(HDC hDC, RECT tRect, float x, float y)
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);
	Rectangle(hDC, (int)tRect.left + x, (int)tRect.top + y, (int)tRect.right + x, (int)tRect.bottom + y);
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);

	DeleteObject(hNewPen);
	return true;
}