// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <Windows.h>
#include <objbase.h>
#include <assert.h>
#include <time.h>
#include <Winsock2.h>
#include <process.h>


// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <deque>
#include <set>
using namespace std;

#include "..\..\..\public\public.h"
#include "..\..\..\public\tools.h"
#include "..\..\..\public\debugnew.h"
#include "..\..\..\public\MsgType.h"
#include "..\..\..\public\GUID.h"
#include "..\bounsserver\game.h"