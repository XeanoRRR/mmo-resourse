#pragma once

/// ����Windows�⡢C����ʱ�⼰Publicͷ�ļ� 

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define _WIN32_WINNT 0x0501

// Windows ͷ�ļ�:
#include <objbase.h>
#include <mmsystem.h>
#include <windows.h>
#include <Winsock2.h>
#include <Commdlg.h>

// C ����ʱͷ�ļ�
#include <assert.h>
#include <process.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <crtdbg.h>
#include <io.h>

#include "DefType.h"
#include "DefSTL.h"
#include "DefCommon.h"
#include "AppFrame/AppFrameDef.h"
