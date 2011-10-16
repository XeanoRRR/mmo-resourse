// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "../DefSTL.h"
#include "../DefPublic.h"

#include "../Log/Log4c.h"
#include "../Log/LogDef.h"
#include "../AppFrame/TextRes.h"

using namespace AppFrame;

#ifndef PutDebugString
#define PutDebugString(module, msg, file, code)
#endif

#ifndef PutErrorString
#define PutErrorString(module, fmt, where, msg)
#endif

#ifndef PutTraceString
#define PutTraceString(module, msg)
#endif

