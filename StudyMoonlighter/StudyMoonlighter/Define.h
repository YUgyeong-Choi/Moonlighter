#pragma once
#define WINCX		1280
#define WINCY		720

#define	PURE		= 0

#define VK_MAX				0xff
#define OBJ_NOEVENT			0
#define OBJ_DEAD			1	

extern HWND		g_hWnd;

enum SCENEID { SC_MENU, SC_VILLAGE, SC_END };
enum OBJID {OBJ_PLAYER, OBJ_END};

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