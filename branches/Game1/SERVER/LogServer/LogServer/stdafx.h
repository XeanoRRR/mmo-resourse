// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4018)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#define _WIN32_WINNT 0x0500
#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

// Windows ͷ�ļ�:
#include <windows.h>
#include <wtypes.h>
#include <WinBase.h>
#include <process.h>


// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

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
#include <deque>
#include <set>

#include "..\..\..\public\public.h"
#include "..\..\..\public\tools.h"
#include "..\..\..\public\readwrite.h"
#include "..\..\..\public\debugnew.h"
#include "..\..\..\public\MsgType.h"

#include "..\..\..\public\NetSession.h"
#include "..\..\..\public\NetSessionManager.h"
#include "..\..\..\public\Timer.h"
#include "..\..\..\public\Date.h"

#include "..\..\..\public\ClientResource.h"
#include "..\..\..\public\RFile.h"
#include "../../../public/LogDef.h"
using namespace std;

extern HWND g_hWnd;