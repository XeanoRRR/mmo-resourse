// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4018)

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�

#include <Windows.h>

#include <assert.h>
#include <time.h>
#include <mmsystem.h>
#include <process.h>

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>


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

#include "..\public\public.h"
#include "..\public\tools.h"
#include "..\public\debugnew.h"
#include "..\public\guid.h"
#include "..\public\RFile.h"
#include "..\public\RCObject.h"
#include "..\public\loadstate.h"
#include "..\public\DataBlockSetWriteRead.h"
