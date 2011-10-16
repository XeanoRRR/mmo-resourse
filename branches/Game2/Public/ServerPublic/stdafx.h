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
// Windows ͷ�ļ�:
#include <objbase.h>
#include <mmsystem.h>
#include <windows.h>
#include <Winsock2.h>
// C ����ʱͷ�ļ�
#include <assert.h>
#include <process.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <winbase.h>

#include "../DefPublic.h"

#include "../Module/CrashDumper.h"
#include "../Module/GUID.h"
#include "../Module/Date.h"
#include "../Module/tools.h"
#include "../Package/RFile.h"
#include "../Package/ClientResource.h"

#include "../tinyxml/tinyxml.h"

#include "../DataStream/DataBlockSetWriteRead.h"

#include "../Common/OrganizingDef.h"
#include "../Common/MsgType.h"
#include "../Common/Public.h"
#include "../Common/BaseDef.h"

#include "ServerPublic.h"

#include "../Log/Log4c.h"
#include "../Log/LogDef.h"
#include "../AppFrame/TextRes.h"
using namespace AppFrame;

