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



class BBB
{
public:
	char arr[128];
	string str;
};


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
