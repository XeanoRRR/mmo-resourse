// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4018)
#pragma warning(disable:4996)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#include <Windows.h>
#include <objbase.h>
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
#include <deque>
#include <set>
using namespace std;

#include "..\..\public\public.h"
#include "..\..\public\tools.h"
#include "..\..\public\readwrite.h"
#include "..\..\public\debugnew.h"
#include "..\..\public\GUID.h"
#include "..\..\server\billingserver\billingserver\game.h"