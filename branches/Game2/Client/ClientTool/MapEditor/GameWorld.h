// GameWorld.h: interface for the GameWorld class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning(disable:4996)

#include <windows.h>

// ��һ�Σ����ܸĽ�����ܼ��𣩣��ڶ��������µ�С���ܣ��������޸�Ŀǰ����������BUG
#ifdef _DEBUG
#define MAPEDITORNAME "Map Editor 3.20.3014 LastUpdate: 2010/08/26 (DEBUG)" 
#else
#define MAPEDITORNAME "Map Editor 3.20.3014 LastUpdate: 2010/08/26 (RELEASE)"
#endif

extern bool g_bDebug;

class GameWorld  
{
	HWND   m_hWnd;
	static GameWorld * m_pInstance;

public:

	BOOL Update();
	void Render();

	static GameWorld * GetInstance();
	static void InitInstance();
	static void ExitInstance();

	void Create(HWND hWnd);
	void Destroy(void);
};