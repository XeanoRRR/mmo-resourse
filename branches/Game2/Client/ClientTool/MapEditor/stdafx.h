// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4996)

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Commdlg.h>
#include <mmsystem.h>
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800


#include <dinput.h>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#include <shlobj.h>

#include <list>
#include <algorithm>
#include <string>
#include <map>
#include <deque>
#include <set>
#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;


#include "..\..\..\public\Package\RFile.h"
#include "..\..\..\public\Package\Clientresource.h"
#include "..\..\..\public\ClientPublic\LoadState.h"
#include "..\..\..\public\Common\RCObject.h"
#include "..\..\..\public\Module\tools.h"

#include "..\..\engine\singleton.h"
#include "..\..\EngineExtend\displaymodel.h"
#include "..\..\EngineExtend\gamemodelmanager.h"
#include "..\..\EngineExtend\gamebuildlist.h"
#include "..\..\EngineExtend\textureprojective.h"


#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")


