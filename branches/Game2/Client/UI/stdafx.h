// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

#include <CEGUI.h>
#include "../../Public/ClientPublic/DefWin.h"
#include "../../Public/ClientPublic/DefD3D.h"
#include "../../Public/DefSTL.h"
#include "../../Public/DefType.h"
#include "../../Public/Common/CStrConverter.h"
#include "../../Public/Module/Tools.h"
#include "../../Public/Package/RFile.h"
#include "../../Public/Module/Singleton.h"

#include "../../Public/Log/Log4c.h"
#include "../../Public/Log/LogDef.h"
#include "../../Public/AppFrame/TextRes.h"
using namespace AppFrame;

extern HWND g_hWnd;

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
