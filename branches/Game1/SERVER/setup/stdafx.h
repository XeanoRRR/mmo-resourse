// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4018;disable:4244;disable:4267;disable:4311;disable:4312)

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

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <mmsystem.h>
#include <assert.h>
#include <math.h>

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <sstream>
using namespace std;

#include "../../public/RFile.h"
#include "../../public/ClientResource.h"
#include "../../public/tools.h"
#include "../../public/public.h"
#include "../../public/readwrite.h"
#include "../../public/DataBlockSetWriteRead.h"

