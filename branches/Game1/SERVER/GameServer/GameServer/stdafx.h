// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define _WIN32_WINNT 0x0501
// Windows ͷ�ļ�:
#include <windows.h>
#include <objbase.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�


// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <deque>
#include <set>
#include <hash_set>
//#include <afxdlgs.h>

#include <mmsystem.h>
#include <assert.h>
#include "../public/tools.h"
#include "../public/readwrite.h"

#include "..\public\NetSession.h"
#include "..\public\NetSessionManager.h"
#include "..\public\MsgType.h"
#include "../public/LogDef.h"
#include "../public/Singleton.h"
#include "..\public\DataBlockSetWriteRead.h"

using namespace std;
using namespace stdext;

extern HWND g_hWnd;