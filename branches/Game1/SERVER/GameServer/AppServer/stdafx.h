// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define _WIN32_WINNT 0x0501
// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�

#include <Windows.h>
#include "objbase.h"
#include <process.h>
#include <assert.h>
#include <time.h>

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <hash_set>
#include <sstream>
#include <deque>
#include <limits>

#include "..\public\public.h"
#include "..\public\ini.h"
#include "..\public\readwrite.h"
#include "..\public\tools.h"
#include "..\public\MsgType.h"
#include "../public/LogDef.h"
#include "GameServer\game.h"
#include "GameServer\GameManager.h"
#include "..\..\nets\netserver\message.h"
#include "..\..\nets\MemClient.h"

#include "..\public\NetSession.h"
#include "..\public\NetSessionManager.h"


#include "..\public\ClientResource.h"
#include "..\public\RFile.h"
#include "..\public\GameEvent.h"

#include "..\LogClient\gs\LogicLogInterface.h"
#include "..\public\StrUnifyInput.h"
#include "..\public\Singleton.h"
#include "..\public\DataBlockSetWriteRead.h"

using namespace std;
using namespace stdext;
