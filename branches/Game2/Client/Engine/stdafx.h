// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include "singleton.h"
//�Զ�������
#include "../../Public/DefPublic.h"
#include "../../Public/ClientPublic/DefD3D.h"
#include "../../Public/Common/RCObject.h"
#include "Math.h"

#include "../../Public/Log/Log4c.h"
#include "../../Public/Log/LogDef.h"
#include "../../Public/AppFrame/TextRes.h"
using namespace AppFrame;

using namespace std;
using namespace stdext;