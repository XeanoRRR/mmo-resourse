// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4996)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define _WIN32_WINNT 0x0500
// Windows ͷ�ļ�:
#include <windows.h>
#include <wtypes.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <assert.h>
#include <time.h>
#include <MMSystem.h>

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <deque>

using namespace std;

#include "..\..\..\public\public.h"
#include "..\..\..\public\tools.h"
#include "..\..\..\public\ini.h"
#include "..\..\..\public\debugnew.h"
#include "..\..\..\public\MsgType.h"
#include "..\..\..\public\GUID.h"
#include "..\..\..\public\timer.h"


extern HWND g_hWnd;