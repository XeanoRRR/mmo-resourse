// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#include <stdio.h>
#include <tchar.h>

#include <process.h>
#include <Windows.h>
#include <assert.h>
#include "time.h"

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <memory>


#include <vector>
using namespace std;


#define SHRINK_SCALE 8
#include "..\MemManage\MemTools.h"


class AAA
{
public:
	AAA(){pLong1 = (long*)M_ALLOC(sizeof(long) * 8);}
	virtual ~AAA(){M_FREE(pLong1, sizeof(long) * 8);}
	long* pLong1;
};

class BBB :public AAA
{
public:
	BBB(){pLong2 = (long*)M_ALLOC(sizeof(long) * 8);}
	virtual ~BBB(){M_FREE(pLong2, sizeof(long) * 8);printf("~BBB()\r\n");}
	char arr[128];
	string str;
	long* pLong2;
};

class CCC
{
public:
	CCC(void *pBBB)
	{
		m_pBBB = (BBB*)pBBB;
	}
	BBB *m_pBBB;
};

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
