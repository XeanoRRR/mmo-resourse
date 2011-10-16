// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4995)
#pragma warning(disable:4267)
#pragma warning(disable:4018)
#pragma warning(disable:4244)

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif	

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif


// Windows ͷ�ļ�:
#include <windows.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�

#include <Windows.h>
#include <assert.h>
#include <time.h>
#include <mmsystem.h>


// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
using namespace std;

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <dshow.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "strmiids.lib")


#include "..\public\public.h"
#include "..\public\tools.h"
#include "..\public\readwrite.h"
#include "..\public\ini.h"
#include "..\public\rfile.h"
#include "..\public\rcobject.h"
#include "..\public\loadstate.h"
#include "..\public\clientresource.h"

#include "..\public\debugnew.h"

#include "..\public\NetSession.h"
#include "..\public\NetSessionManager.h"
#include "..\public\MsgType.h"
#include "..\public\DataBlockSetWriteRead.h"

#include "..\input\mouse.h"
#include "..\input\keyboard.h"
#include "gameclient\game.h"

extern HWND g_hWnd;