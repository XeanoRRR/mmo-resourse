// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4996)


#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#ifdef _DEBUG
#define ENABLE_ALOADER
#endif
#include "../../../Public/Package/ClientResLoader.h"
#include "../../../Public/DefPublic.h"
#include "../../../Public/ClientPublic/ClientPublic.h"
#include "../../../public/Common/MsgType.h"
#include "../../../public/Common/Public.h"
#include "../../UI/UIDef.h"

#include "../../../Public/Log/Log4c.h"
#include "../../../Public/Log/LogDef.h"
#include "../../../Public/AppFrame/TextRes.h"
using namespace AppFrame;


extern HWND g_hWnd;