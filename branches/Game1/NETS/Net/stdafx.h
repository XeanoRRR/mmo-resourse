// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#pragma warning(disable:4996)

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <Windows.h>
#include <wtypes.h>
#include <assert.h>
#include <time.h>
#include <process.h>

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <deque>


#include "..\..\public\public.h"
#include "..\..\public\tools.h"
#include "..\..\public\readwrite.h"
#include "..\..\public\debugnew.h"
#include "..\..\public\DataBlockSetWriteRead.h"
#include "..\..\server\server\server\game.h"

using namespace std;
