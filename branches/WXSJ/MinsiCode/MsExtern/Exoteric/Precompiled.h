// Precompiled.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

#ifdef _INTEL
#    pragma warning(disable : 810)
#    pragma warning(disable : 447)
#    pragma warning(disable : 324)
#    pragma warning(disable : 693)
#    pragma warning(disable : 1786)
#    pragma warning(disable : 1684)
#    pragma warning(disable : 181)
#    pragma warning(disable : 967)
#else
#    pragma warning(disable : 4996)
#    pragma warning(disable : 4201)
#    pragma warning(disable : 4611)
#    pragma warning(disable : 4706)
#    pragma warning(disable : 4244)
#    pragma warning(disable : 4311)
#    pragma warning(disable : 4311)
#    pragma warning(disable : 4312)
#    pragma warning(disable : 4245)
#    pragma warning(disable : 4189)
#    pragma warning(disable : 4505)
#    pragma warning(disable : 4702)
#    pragma warning(disable : 4328)
#    pragma warning(disable : 4310)
#    pragma warning(disable : 4366)
#endif

extern "C"
{
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
}

#include <queue>

#include <windows.h>
#include <sys/types.h>
#include <string.h>
#include <basetsd.h>	/* Davide Pizzolato: correct type definition for VC6 */
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <locale.h>
#include <limits.h>
#include <time.h>
#include <io.h>
#include <crtdbg.h>
#include <fcntl.h>
#include <float.h>
#include <tchar.h>
#include <MMSystem.h>
#include <ShellAPI.h>

#if     (_OPENMP == 0)
#   error  �޷�ʶ��������汾, ��򿪹��� -> C/C++ -> ���� -> OpenMP ֧�� -> ��
#elif   (_OPENMP == 200805) && !defined(_INTEL)
#   error  Intel����������ʹ��Intel����ѡ����б���!
#elif   (_OPENMP == 200203) && defined(_INTEL)
#   error  Visual C++����������ʹ��VC����ѡ����б���!
#endif
