// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4018;disable:4244;disable:4267;disable:4311;disable:4312;disable:4311;disable:4996)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#define _WIN32_DCOM
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
#include <hash_map>
#include <hash_set>
//#include <afxdlgs.h>
using namespace std;
using namespace stdext;

#include <mmsystem.h>
#include <assert.h>

#include "debugnew.h"
#include "rfile.h"
#include "rcobject.h"
#include "loadstate.h"
#include "clientresource.h"
#include "tools.h"
#include "DataBlockSetWriteRead.h"