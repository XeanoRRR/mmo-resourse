// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4018)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4313)


#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0500	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#include <assert.h>

// Windows ͷ�ļ�:
#include <windows.h>
#include <wtypes.h>
#include <WinBase.h>
#include <process.h>
#include <MMSystem.h>
#include "time.h"

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <deque>
using namespace std;


// �Զ����
#include "../../../public/tools.h"
#include "../../../public/public.h"
#include "../../../public/GUID.h"
#include "../../../public/LogDef.h"
#include "../../../public/tinystr.h"
#include "../../../public/tinyxml.h"

#include "LogServer\game.h"