#pragma once              //��                                          //��
#ifndef __MSBASEDEF_H__   //��                                          //��
#define __MSBASEDEF_H__   //��                                          //��
//����������������������������                                          //��
//�ļ����ƣ�MsBaseDef.h                                                 //��
//�����������������Ͷ��壬����ͷ�ļ������������궨�壬ȫ�ֳ�ʼ�����ͷ�  //��
//ƽ̨��أ�WINDOWS                                                     //��
//��������������������������������������������������������������������������

//��������������������������������������������������������������������������
#ifndef I_KNOW_THE_BASE_TYPE                                            //��
#error ��鿴MyBaseType.h�ļ������ӡ�I_KNOW_THE_BASE_TYPE����ȥ��������
#endif                                                                  //��
                                                                        //��
#ifdef NO_COMPILER_MESSAGE                                              //��
#pragma message("ע��:���ѽ��ñ�����Ϣ!")                               //��
#pragma message("��鿴�������,��ȥ����NO_COMPILER_MESSAGE��")         //��
#endif                                                                  //��
#if defined(DLL_EXPORTS)                                                //��
#define USE_DLL __declspec(dllexport)                                   //��
#else                                                                   //��
#define USE_DLL __declspec(dllimport)                                   //��
#endif                                                                  //��
                                                                        //��
#ifndef WINVER                                                          //��
#define WINVER 0x0500                                                   //��
#endif                                                                  //��
                                                                        //��
// ��ʹ��MFC                                                            //��
#ifndef USE_MFC                                                         //��
// �����׽���                                                           //��
#include <Winsock2.h>                                                   //��
#define NO_MFC_CODE(code) code                                          //��
#else                                                                   //��
#include <afxsock.h>    // MFC �׽�����չ                               //��
#define NO_MFC_CODE(code)                                               //��
#endif                                                                  //��
//��������������������������������������������������������������������������


//����������������������������������������������
#ifdef USE_MS_DX                            //��
#define USE_MS_QUEUE_MANAGER                //��
#include "d3d9.h"                           //��
#include "d3dx9.h"                          //��
#include "d3d10.h"                          //��
#include "d3dx10.h"                         //��
#include "dds.h"                            //��
#ifndef MS_LIB                              //��
#pragma comment(lib, "d3d9.lib")            //��
#pragma comment(lib, "d3d10.lib")           //��
#pragma comment(lib, "dxguid.lib")          //��
#pragma comment(lib, "dinput8.lib")         //��
#   ifdef _DEBUG                            //��
#       pragma comment(lib, "d3dx9d.lib")   //��
#       pragma comment(lib, "d3dx10d.lib")  //��
#   else                                    //��
#       pragma comment(lib, "d3dx9.lib")    //��
#       pragma comment(lib, "d3dx10.lib")   //��
#   endif                                   //��
#endif                                      //��
#endif                                      //��
//����������������������������������������������

//������������������������������
//������������ͷ�ļ�        //��
//--------------------------//��
// ��׼�������             //��
#include <Stdio.h>          //��
                            //��
// C++���ļ�                //��
#include <Stdlib.h>         //��
                            //��
// �ַ�ͷ                   //��
#include <Tchar.h>          //��
                            //��
// �ַ���ͷ                 //��
#include <String>           //��
                            //��
// GDI+ͷ�ļ�               //��
#include <Gdiplus.h>        //��
                            //��
// MFC_DAO                  //��
#ifdef USE_MFC              //��
#define _AFX_DISABLE_DEPRECATED
#include <afx.h>            //��
#else                       //��
                            //��
// ��׼����                 //��
#include <Errno.h>          //��
                            //��
// ��׼�������             //��
#include <Iostream>         //��
                            //��
// ʱ��ͷ                   //��
#include <Time.h>           //��
                            //��
// ��ѧ��ʽͷ               //��
#include <Math.h>           //��
                            //��
// ����ͷ                   //��
#include <Queue>            //��
                            //��
// ����ͷ                   //��
#include <Assert.h>         //��
                            //��
// �߳�ͷ                   //��
#include <Process.h>        //��
                            //��
// ����̨���               //��
#include <Conio.h>          //��
                            //��
// Windowsͷ�ļ�            //��
#include <Windows.h>        //��
                            //��
// Windows���̹���          //��
#include <Psapi.h>          //��
                            //��
// Windows��������          //��
#include <Tlhelp32.h>       //��
                            //��
#endif                      //��
//������������������������������

//��������������������������������������������������
//��׼�������Ͷ���                              //��
                                                //��
typedef double      DOUBLE;     //˫����        //��
typedef INT64       IP;         //IP����        //��
typedef INT64       ID;         //ID����        //��
typedef INT         NFLAG;      //�������      //��
typedef INT         NPOS;       //��������      //��
typedef INT         NSIZE;      //�ߴ�����      //��
typedef UINT        UFLAG;      //�������      //��
typedef UINT        UPOS;       //��������      //��
typedef UINT        USIZE;      //�ߴ�����      //��
typedef FLOAT       FFLAG;      //�������      //��
typedef FLOAT       FPOS;       //��������      //��
typedef FLOAT       FSIZE;      //�ߴ�����      //��
typedef CHAR        ACHAR;      //���ַ��ض���  //��
typedef TCHAR*      LPTCHAR;    //��(��)�ַ�    //��
typedef ACHAR*      LPCHAR;     //�ַ�ָ��      //��
typedef WCHAR*      LPWCHAR;    //���ַ�ָ��    //��
typedef SHORT*      LPSHORT;    //����ָ��      //��
typedef INT*        LPINT;      //����ָ��      //��
typedef LONG*       LPLONG;     //����ָ��      //��
typedef INT64*      LPINT64;    //64��ָ��      //��
typedef FLOAT*      LPFLOAT;    //����ָ��      //��
typedef DOUBLE*     LPDOUBLE;   //˫��ָ��      //��
typedef UCHAR*      LPUCHAR;    //�޷����ַ�    //��
typedef USHORT*     LPUSHORT;   //�޷��Ŷ���    //��
typedef UINT*       LPUINT;     //�޷�������    //��
typedef ULONG*      LPULONG;    //�޷��ų���    //��
typedef UINT64*     LPUINT64;   //�޷���64��    //��
typedef BOOL*       LPBOOL;     //����ָ��      //��
typedef BYTE*       LPBYTE;     //�ֽ�ָ��      //��
typedef IP*         LPIP;       //IP����ָ��    //��
typedef ID*         LPID;       //ID����ָ��    //��
typedef NFLAG*      LPNFLAG;    //�������      //��
typedef NPOS*       LPNPOS;     //��������      //��
typedef UFLAG*      LPUFLAG;    //�������      //��
typedef UPOS*       LPUPOS;     //��������      //��
//��������������������������������������������������

//����������������������������������
// �������                     //��
#include "MsBaseErrorCode.h"    //��
                                //��
// �߳���Ϣ                     //��
#include "MsThreadMessageDef.h" //��
//����������������������������������

//����������������������������������������������������������
//�������ݺ궨��                                        //��
                                                        //��
//IP��ַ���ַ���󳤶�                                  //��
#ifndef IP_SIZE                                         //��
#define IP_SIZE         16                              //��
#endif                                                  //��
                                                        //��
//��Ч�ľ��                                            //��
#ifndef INVALID_HANDLE                                  //��
#define INVALID_HANDLE  INVALID_HANDLE_VALUE            //��
#endif                                                  //��
                                                        //��
//��Ч��IDֵ                                            //��
#define INVALID_NID     -1                              //��
#define INVALID_UID     (DWORD)-1                       //��
#define INVALID_WID     (WORD)-1                        //��
#define INVALID_BID     (BYTE)-1                        //��
                                                        //��
#define GB_SIZE         (MB_SIZE*1024)                  //��
#define MB_SIZE         (KB_SIZE*1024)                  //��
#define KB_SIZE         (BYTE_SIZE*1024)                //��
#define BYTE_SIZE       (sizeof(BYTE))                  //��
                                                        //��
//��Ч��ָ��ֵ                                          //��
#ifndef INVALID_POINTER                                 //��
#define INVALID_POINTER (VOID*)INVALID_NID              //��
#endif                                                  //��
                                                        //��
// �궨���������                                       //��
#define USE_PARAM(...)(__VA_ARGS__)                     //��
                                                        //��
#ifdef UNICODE                                          //��
#define _tcout std::wcout                               //��
#define _tcin  std::wcin                                //��
#else                                                   //��
#define _tcout std::cout                                //��
#define _tcin  std::cin                                 //��
#endif                                                  //��
                                                        //��
//�����ı�����                                          //��
#ifndef MAX_ERROR_TEXT_LEN                              //��
#define MAX_ERROR_TEXT_LEN      160                     //��
#endif                                                  //��
                                                        //��
// ����ת�����ַ������賤��                             //��
#define WORD_NUMBER_SIZE        6                       //��
#define DWORD_NUMBER_SIZE       11                      //��
#define FWORD_NUMBER_SIZE       21                      //��
#define MAX_USER_NAME_LEN       20                      //��
#define MAX_USER_PASSWORD_LEN   32                      //��
                                                        //��
// ���ƺ궨��                                           //��
#define RADIX_2 2       // 2���Ƶĺ궨��                //��
#define RADIX_8 8       // 8���Ƶĺ궨��                //��
#define RADIX_10 10     // 10���Ƶĺ궨��               //��
#define RADIX_16 16     // 16���Ƶĺ궨��               //��
                                                        //��
// 16λ����ת��8�����ַ�����󳤶�                      //��
#define N16_TO_S_MAX_LEN_BY_R8  7                       //��
                                                        //��
// 16λ����ת��10�����ַ�����󳤶�                     //��
#define N16_TO_S_MAX_LEN_BY_R10 6                       //��
                                                        //��
// 16λ����ת��16�����ַ�����󳤶�                     //��
#define N16_TO_S_MAX_LEN_BY_R16 5                       //��
                                                        //��
// 32λ����ת��8�����ַ�����󳤶�                      //��
#define N32_TO_S_MAX_LEN_BY_R8  12                      //��
                                                        //��
// 32λ����ת��10�����ַ�����󳤶�                     //��
#define N32_TO_S_MAX_LEN_BY_R10 11                      //��
                                                        //��
// 32λ����ת��16�����ַ�����󳤶�                     //��
#define N32_TO_S_MAX_LEN_BY_R16 9                       //��
                                                        //��
// 64λ����ת��8�����ַ�����󳤶�                      //��
#define N64_TO_S_MAX_LEN_BY_R8  23                      //��
                                                        //��
// 64λ����ת��10�����ַ�����󳤶�                     //��
#define N64_TO_S_MAX_LEN_BY_R10 21                      //��
                                                        //��
// 64λ����ת��16�����ַ�����󳤶�                     //��
#define N64_TO_S_MAX_LEN_BY_R16 17                      //��
                                                        //��
#define FOR_x_TO(x,end) for(DWORD x=0; x<end; x++)      //��
                                                        //��
#define NO_CONTINUOUS   ::SleepEx(1, TRUE)              //��
                                                        //��
#ifdef _DEBUG                                           //��
#define _DEBUG_CODE(code) code                          //��
#else                                                   //��
#define _DEBUG_CODE(code)                               //��
#endif                                                  //��
                                                        //��
                                                        //��
#define MS_LOCK_BEGIN   // �˺궨�忪ʼ˵���˶δ������ //��
                    // ���⺬��,���ԸĶ�������������!   //��
                                                        //��
#define MS_LOCK_END     // �˺궨��֮ǰ˵���˶δ������ //��
                    // ���⺬��,���ԸĶ�������������!   //��
                                                        //��
#define MS_LOFTY    // �˺궨�����˴����ڸ߼�����,    //��
                    // ����ϸ�˽������ļܹ�����ʹ��.    //��
                                                        //��
#define P_OPER_S(p1,p2) ((SIZE_T)(p1)-(SIZE_T)(p2))     //��
#define P_OPER_A(p1,p2) ((SIZE_T)(p1)+(SIZE_T)(p2))     //��
#define P_OPER_D(p1,p2) ((SIZE_T)(p1)/(SIZE_T)(p2))     //��
#define P_OPER_M(p1,p2) ((SIZE_T)(p1)*(SIZE_T)(p2))     //��
                                                        //��
// ��BOOL����ת����Сbool���͵ĺ궨��                   //��
#define BOOL_To_bool(val)           ((val)?true:false)  //��
                                                        //��
#define MSCALL  __fastcall                              //��
                                                        //��
#define MS_DEPRECATEE           __declspec(deprecated)  //��        
                                                        //��
typedef INT (MSCALL *RUNCALLBACK)(INT,LPTCHAR*);        //��
typedef BOOL (MSCALL *HOOKWNDPROC)(MSG*);               //��
//��������������������������������������������������������������������������
//���������궨��                                                        //��
                                                                        //��
//����ָ��ֵɾ���ڴ�                                                    //��
#ifndef SAFE_DELETE                                                     //��
#define SAFE_DELETE(p)      ((p) ? (delete (p), (p)=NULL) : ((p) = (p)))//��
#endif                                                                  //��
                                                                        //��
//����ָ��ֵɾ�����������ڴ�                                            //��
#ifndef SAFE_DELETE_ARRAY                                               //��
#define SAFE_DELETE_ARRAY(p)    ((p)?(delete[] (p),(p)=NULL):((p)=(p))) //��
#endif                                                                  //��
                                                                        //��
//����ָ��ֵ�ͷŶ���                                                    //��
#ifndef SAFE_RELEASE                                                    //��
#define SAFE_RELEASE(p)     ((p)?((p)->Release(),(p)=NULL):((p) = (p))) //��
#endif                                                                  //��
                                                                        //��
// ��������                                                             //��
#ifndef EXCHANGE_DATA                                                   //��
#define EXCHANGE_DATA(a,b)      (a) ^= (b), (b) ^= (a), (a) ^= (b);     //��
#endif                                                                  //��
                                                                        //��
// �Զ����ȳ�ʼ���ṹ��                                                 //��
#ifndef ZeroMemoryStruct                                                //��
#define ZeroMemoryStruct(p)   ZeroMemory(&(p), sizeof(p))               //��
#endif                                                                  //��
                                                                        //��
// �Զ����ȳ�ʼ������(����ԭʼ����,ָ���������)                        //��
#ifndef ZeroMemoryArray                                                 //��
#define ZeroMemoryArray(a)   ZeroMemory((a), sizeof(a))                 //��
#endif                                                                  //��
//��������������������������������������������������������������������������

//��������������������������������������������������������������������������
//Windows�ṹ���쳣                                                     //��
                                                                        //��
#ifndef NO_THROW                                                        //��
// ���뺯��                                                             //��
#define _ENTER_FUN                  {__try{                             //��
#define _BENTER_FUN(b,ret)          if(!b)return ret;{__try{            //��
#define _ENTER_FUN_C                {try{                               //��
#define _BENTER_FUN_C(b,ret)        if(!b)return ret;{try{              //��
                                                                        //��
// �뿪����                                                             //��
#define _LEAVE_FUN                  }                                   //��
#define _LEAVE_FUN_C                }                                   //��
                                                                        //��
// �����쳣����ֹ�����׳�                                               //��
#define _DISPOSE_END                }__except(EXCEPTION_EXECUTE_HANDLER)//��
#define _DISPOSE_CUSTOM_END(type)   }__except(type)                     //��
#define _DISPOSE_END_C              }catch(...)                         //��
#define _DISPOSE_CUSTOM_END_C(type) }catch(type)                        //��
#define _LEAVE_FUN_DISPOSE_END      _DISPOSE_END{} _LEAVE_FUN           //��
#define _LEAVE_FUN_DISPOSE_END_C    _DISPOSE_END_C{} _LEAVE_FUN         //��
                                                                        //��
// �����쳣����                                                         //��
#define _DISPOSE_REPEAT     }__except(EXCEPTION_CONTINUE_EXECUTION)     //��
#define _LEAVE_FUN_DISPOSE_REPEAT   _DISPOSE_REPEAT{} _LEAVE_FUN        //��
                                                                        //��
// �����������쳣                                                       //��
#define _DISPOSE_CONTINUE           }__except(EXCEPTION_CONTINUE_SEARCH)//��
#define _DISPOSE_CONTINUE_C         }catch(...)                         //��
#define _LEAVE_FUN_DISPOSE_CONTINUE     _DISPOSE_CONTINUE{} _LEAVE_FUN  //��
#define _LEAVE_FUN_DISPOSE_CONTINUE_C   _DISPOSE_CONTINUE_C{} _LEAVE_FUN//��
                                                                        //��
// ������ʾ�쳣                                                         //��
#define _LEAVE_FUN_ONLY_CUE     _DISPOSE_CONTINUE{\
                                AssertEx(AL_NORMAL, FALSE, _T(__FUNCTION__));\
                                }_LEAVE_FUN                             //��
#define _LEAVE_FUN_ONLY_CUE_C   _DISPOSE_CONTINUE_C{\
                                AssertEx(AL_NORMAL, FALSE, _T(__FUNCTION__));\
                                }_LEAVE_FUN                             //��
                                        //����������������������������������
#else                                   //��
                                        //��
#pragma warning(disable : 4702)         //��
// ���뺯��                             //��
#define _ENTER_FUN                      //��
#define _ENTER_FUN_C                    //��
                                        //��
// �뿪����                             //��
#define _LEAVE_FUN                      //��
#define _LEAVE_FUN_C                    //��
                                        //��
// �����쳣����ֹ�����׳�               //��
#define _DISPOSE_END                    //��
#define _DISPOSE_END_C                  //��
#define _LEAVE_FUN_DISPOSE_END          //��
#define _LEAVE_FUN_DISPOSE_END_C        //��
                                        //��
// �����쳣����                         //��
#define _DISPOSE_REPEAT                 //��
#define _LEAVE_FUN_DISPOSE_REPEAT       //��
                                        //��
// �����������쳣                       //��
#define _DISPOSE_CONTINUE               //��
#define _DISPOSE_CONTINUE_C             //��
#define _LEAVE_FUN_DISPOSE_CONTINUE     //��
#define _LEAVE_FUN_DISPOSE_CONTINUE_C   //��
                                        //��
// ������ʾ�쳣                         //��
#define _LEAVE_FUN_ONLY_CUE             //��
#define _LEAVE_FUN_DISPOSE_CONTINUE_C   //��
                                        //��
#endif                                  //��
//������������������������������������������

// ��Զ��ʹ�õ�
#define _ENTER_FUN_FOREVER  {__try{
// �����Ƿ��쳣,�뿪��������ִ��
#define _DISPOSE_MUST_RUN   }__finally
#define _LEAVE_FUN_DISPOSE_MUST_RUN _DISPOSE_MUST_RUN{} }

//������������������������������������������������������������������
//���µ�ǰʱ�䵽ȫ�ֱ���                                        //��
extern SYSTEMTIME g_LocalTime;                                  //��
#if !defined(UPDATA_LOCALTIME) && !defined(LOCALTIME_YEAR)      //��
#define UPDATA_LOCALTIME        ::GetLocalTime(&g_LocalTime)    //��
#define LOCALTIME_YEAR          g_LocalTime.wYear               //��
#define LOCALTIME_MONTH         g_LocalTime.wMonth              //��
#define LOCALTIME_DAY           g_LocalTime.wDay                //��
#define LOCALTIME_WEEK          g_LocalTime.wDayOfWeek          //��
#define LOCALTIME_HOUR          g_LocalTime.wHour               //��
#define LOCALTIME_MINUTE        g_LocalTime.wMinute             //��
#define LOCALTIME_SECOND        g_LocalTime.wSecond             //��
#define LOCALTIME_MILLISECOND   g_LocalTime.wMilliseconds       //��
#define UPDATA_FILETIME(ft)     UPDATA_LOCALTIME;\
    ::SystemTimeToFileTime(&g_LocalTime, (FILETIME*)(&ft));

#define UPDATA_FILETIME_MILLISECOND(ft) \
    UPDATA_FILETIME(ft); ft/=10000;
#endif                                                          //��
//������������������������������������������������������������������

//������������������������������������������������������������������
//���ʹ�õ�Ƶ�ʣ�����Խ��ͬһʱ��ʹ������������뼸��Խ��      //��
#define RFREQUENCY 4096                                         //��
                                                                //��
//ȫ�ַ����������������                                        //��
extern DOUBLE   g_dRandRadix[RFREQUENCY];                       //��
                                                                //��
//ȫ�ַ����������������                                        //��
extern UINT     g_uRandRadixIndex;                              //��
                                                                //��
//�������������                                                //��
#ifndef DOUBLE_RAND                                             //��
#define DOUBLE_RAND ((DOUBLE)rand())                            //��
#endif                                                          //��
                                                                //��
//����������ӣ����������ͬһʱ�������ɲ�ͬ����                //��
inline DOUBLE MY_RAND(DOUBLE& Radix)                            //��
{                                                               //��
    UPDATA_LOCALTIME;                                           //��
        (Radix) =                                               //��
        (DOUBLE_RAND+LOCALTIME_MILLISECOND+LOCALTIME_SECOND)/   //��
        (RAND_MAX+DOUBLE_RAND/                                  //��
            (RAND_MAX+LOCALTIME_MILLISECOND+LOCALTIME_SECOND)   //��
        );                                                      //��
        (Radix) += g_dRandRadix[g_uRandRadixIndex++%RFREQUENCY];//��
        (Radix) *= 100;                                         //��
        (Radix) = (Radix)-(INT)(Radix);                         //��
    return (Radix);                                             //��
}                                                               //��
inline DOUBLE MY_RAND()                                         //��
{                                                               //��
    DOUBLE Radix;                                               //��
    UPDATA_LOCALTIME;                                           //��
        (Radix) =                                               //��
        ( DOUBLE_RAND+LOCALTIME_MILLISECOND+LOCALTIME_SECOND )/ //��
        (RAND_MAX+DOUBLE_RAND/                                  //��
            (RAND_MAX+LOCALTIME_MILLISECOND+LOCALTIME_SECOND)   //��
        );                                                      //��
        (Radix) += g_dRandRadix[g_uRandRadixIndex++%RFREQUENCY];//��
        (Radix) *= 100;                                         //��
        (Radix) = (Radix)-(INT)(Radix);                         //��
    return (Radix);                                             //��
}                                                               //��
                                                                //��
//�����������Ӳ��������                                        //��
#define RADIX(Radix)    MY_RAND(Radix)                          //��
                                                                //��
//�������������Ӳ��������                                      //��
#define RADIXN          MY_RAND()                               //��
//������������������������������������������������������������������

//������������������������������������������������������������������
//����������ָ����Χ�����TCHAR                                 //��
#ifndef R_BYTE                                                  //��
#define R_BYTE(rx,ri,ra)    (TCHAR)(rx*(ra-ri)+(ri))            //��
#define R_BYTE_A(ri,ra)     (TCHAR)(RADIXN*(ra-ri)+(ri))        //��
#endif                                                          //��
                                                                //��
//����������ָ����Χ�����WORD                                  //��
#ifndef R_WORD                                                  //��
#define R_WORD(rx,ri,ra)    (WORD)(rx*(ra-ri)+(ri))             //��
#define R_WORD_A(ri,ra)     (WORD)(RADIXN*(ra-ri)+(ri))         //��
#endif                                                          //��
                                                                //��
//����������ָ����Χ�����DWORD                                 //��
#ifndef R_DWORD                                                 //��
#define R_DWORD(rx,ri,ra)   (DWORD)(rx*(ra-ri)+(ri))            //��
#define R_DWORD_A(ri,ra)    (DWORD)(RADIXN*(ra-ri)+(ri))        //��
#endif                                                          //��
                                                                //��
//����������ָ����Χ�����FLOAT                                 //��
#ifndef R_FLOAT                                                 //��
#define R_FLOAT(rx,ri,ra)   (FLOAT)(rx*(ra-ri)+(ri))            //��
#define R_FLOAT_A(ri,ra)    (FLOAT)(RADIXN*(ra-ri)+(ri))        //��
#endif                                                          //��
                                                                //��
//����������ָ����Χ�����DOUBLE                                //��
#ifndef R_DOUBLE                                                //��
#define R_DOUBLE(rx,ri,ra)  (DOUBLE)(rx*(ra-ri)+(ri))           //��
#define R_DOUBLE_A(ri,ra)   (DOUBLE)(RADIXN*(ra-ri)+(ri))       //��
#endif                                                          //��
                                                                //��
//����������ָ����Χ�����INT64                                 //��
#ifndef R_INT64                                                 //��
#define R_INT64(rx,ri,ra)   (INT64)(rx*(ra-ri)+(ri))            //��
#define R_INT64_A(ri,ra)    (INT64)(RADIXN*(ra-ri)+(ri))        //��
#endif                                                          //��
                                                                //��
//һ�����ʵ���ָ��ֵ                                            //��
#ifndef R_ODDS                                                  //��
#define R_ODDS(ri,ra,va,vb) ((RADIXN*(ra))<=(ri))?(va):(vb)     //��
#endif                                                          //��
//������������������������������������������������������������������

//������������������������������������������������������������������
//������                                                        //��
class MsLock                                                    //��
{                                                               //��
public :                                                        //��
    //�����г�ʼ��������                                      //��
    MsLock();                                                   //��
                                                                //��
    //������ɾ��������                                        //��
    ~MsLock();                                                  //��
                                                                //��
    //�����ٽ���                                                //��
    VOID ReSet();                                               //��
                                                                //��
    //�����ٽ���                                                //��
    VOID Lock();                                                //��
                                                                //��
    //�뿪�ٽ���                                                //��
    VOID UnLock();                                              //��
private:                                                        //��
    //������                                                  //��
    CRITICAL_SECTION m_Lock;                                    //��
};                                                              //��
                                                                //��
//�Զ�����������                                                //��
class AutoLock                                                  //��
{                                                               //��
public:                                                         //��
    //�����л�ù�����������                                    //��
    AutoLock(MsLock& rLock, BOOL bUse = TRUE);                  //��
                                                                //��
    //�����н���                                                //��
    ~AutoLock();                                                //��
private:                                                        //��
                                                                //��
    //Ĭ�Ϲ����ֹʹ��                                          //��
    AutoLock();                                                 //��
                                                                //��
    //������ָ��                                                //��
    MsLock* m_pLock;                                            //��
    BOOL    m_bUse;                                             //��
};                                                              //��
                                                                //��
//����������������������������������������������������������������������
//�Ѿ��й����ʾʱ�������������Զ��������뿪�������Զ�����          //��
#define AUTO_LOCK(lock) AutoLock __LINE__##lock(lock)               //��
#define IS_AUTO_LOCK(lock,buse) AutoLock __LINE__##lock(lock,buse)  //��
//����������������������������������������������������������������������

//������������������������������������������������������������������
// ���ӳ�Ա����                                                 //��
class CPoolMember                                               //��
{                                                               //��
public:                                                         //��
    CPoolMember(VOID)                                           //��
    : m_dwIndexInPool   (INVALID_UID)                           //��
    , m_dwIndexInLocate (INVALID_UID)                           //��
    , m_n64EnterIndex   (INVALID_NID)                           //��
    , m_lpData          (NULL)                                  //��
    , m_dwDataSize      (0){}                                   //��
    virtual ~CPoolMember(VOID){};                               //��
    virtual DWORD  GetIndex(VOID){return m_dwIndexInPool;}      //��
    virtual LPVOID GetData(VOID){return m_lpData;}              //��
    virtual DWORD  GetDataSize(VOID){return m_dwDataSize;}      //��
    virtual LPFILETIME  GetLandTime(VOID)                       //��
    {return (LPFILETIME)&m_n64EnterIndex;}                      //��
                                                                //��
    VOID CopyMember(CPoolMember* pMember){CopyMember(*pMember);}//��
    VOID CopyMember(CPoolMember& Member)                        //��
    {                                                           //��
        Member.m_dwIndexInPool      = m_dwIndexInPool;          //��
        Member.m_dwIndexInLocate    = m_dwIndexInLocate;        //��
        Member.m_n64EnterIndex      = m_n64EnterIndex;          //��
        Member.m_lpData             = m_lpData;                 //��
        Member.m_dwDataSize         = m_dwDataSize;             //��
    }                                                           //��
    friend class CMsPoolManager;                                //��
protected:                                                      //��
    virtual BOOL   SetData(LPVOID lpData, DWORD dwDataSize) = 0;//��
    DWORD   m_dwIndexInPool;                                    //��
    DWORD   m_dwIndexInLocate;                                  //��
    INT64   m_n64EnterIndex;                                    //��
    LPVOID  m_lpData;                                           //��
    DWORD   m_dwDataSize;                                       //��
};                                                              //��
                                                                //��
class CNoDataPoolMember                                         //��
    : public CPoolMember                                        //��
{                                                               //��
private:                                                        //��
    virtual BOOL SetData(LPVOID/*lpData*/,DWORD/*dwDataSize*/)  //��
    {return TRUE;}                                              //��
};                                                              //��
//������������������������������������������������������������������

//������������������������������������������������������������������
//ʹ�ô�����NEW                                               //��
#ifndef NEW                                                     //��
    #if defined(_DEBUG)         // ���԰汾                     //��
        #if defined(USE_MFC)                                    //��
            #define NEW new                                     //��
        #else                                                   //��
            #define NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)  //��
        #endif                                                  //��
    #else                       // �����汾                     //��
        #if defined(USE_MFC)                                    //��
            #define NEW new                                     //��
        #else                                                   //��
            #define NEW new(std::nothrow)                       //��
        #endif                                                  //��
    #endif                                                      //��
#endif                                                          //��
//������������������������������������������������������������������

//������������������������������������������������������������������
//��ֹ����ָ��ָ�����ʹ�ô���Ļ���                          //��
class BaseClass                                                 //��
{                                                               //��
public:                                                         //��
    enum E_CLASS_NAME                                           //��
    {                                                           //��
        CLASS_NAME_BASECLASS = 0xFFFFFFFFFFFFFFFF,              //��
    };                                                          //��
    BaseClass():m_ClassFlag(0){;}                               //��
    virtual ~BaseClass() = 0;                                   //��
protected:                                                      //��
    BOOL IsLegal(E_CLASS_NAME ClassFlag)                        //��
    {                                                           //��
        return (0 != (m_ClassFlag & ClassFlag));                //��
    }                                                           //��
    INT64 m_ClassFlag;                                          //��
};                                                              //��
//������������������������������������������������������������������
// GDI+ͼƬ����                                                 //��
struct MsImage                                                  //��
{                                                               //��
    MsImage(TCHAR* szImageFileName)                             //��
        : m_Image(szImageFileName){}                            //��
    MsImage(HBITMAP hBitmap, HPALETTE hPalette = NULL)          //��
        : m_Image(hBitmap, hPalette){}                          //��
    Gdiplus::Bitmap m_Image;                                    //��
};                                                              //��
                                                                //��
// GDI+��������                                                 //��
struct MsGraphics                                               //��
{                                                               //��
    MsGraphics(HDC hDc)                                         //��
        : m_Graphics(hDc){}                                     //��
    Gdiplus::Graphics m_Graphics;                               //��
};                                                              //��
//������������������������������������������������������������������
//ָ���ά�����ָ��ģ��                                        //��
template <class T>                                              //��
class PlanarPoINTer                                             //��
{                                                               //��
public:                                                         //��
    PlanarPoINTer(T* p,UINT nDimensionA, UINT nDimensionB)      //��
        :m_p(p)                                                 //��
        ,m_nDimensionA(nDimensionA)                             //��
        ,m_nDimensionB(nDimensionB)                             //��
    {                                                           //��
        ;                                                       //��
    }                                                           //��
    ~PlanarPoINTer(VOID)                                        //��
    {                                                           //��
        ;                                                       //��
    }                                                           //��
    T* operator [](T i)                                         //��
    {                                                           //��
        return &(m_p[i*m_nDimensionB]);                         //��
    }                                                           //��
    T* operator ++(INT)                                         //��
    {                                                           //��
        m_p += m_nDimensionA * m_nDimensionB;                   //��
        return m_p;                                             //��
    }                                                           //��
    T* operator --(INT)                                         //��
    {                                                           //��
        m_p -= m_nDimensionA * m_nDimensionB;                   //��
        return m_p;                                             //��
    }                                                           //��
    T* operator +=(INT nExcursion)                              //��
    {                                                           //��
        m_p += m_nDimensionA * m_nDimensionB * nExcursion;      //��
        return m_p;                                             //��
    }                                                           //��
    T* operator -=(INT nExcursion)                              //��
    {                                                           //��
        m_p -= m_nDimensionA * m_nDimensionB * nExcursion;      //��
        return m_p;                                             //��
    }                                                           //��
    T*              m_p;                                        //��
    UINT    m_nDimensionA;                                      //��
    UINT    m_nDimensionB;                                      //��
};                                                              //��
typedef PlanarPoINTer<INT> INT_ARRAY_POINTER;                   //��
//������������������������������������������������������������������

//������������������������������·����������������������������������
#ifdef UNICODE                                                  //��
#define CreateFullPath CreateFullPathW                          //��
#else                                                           //��
#define CreateFullPath CreateFullPathA                          //��
#endif                                                          //��
inline BOOL CreateFullPathW(WCHAR* szBuff, WCHAR* szFile)       //��
{                                                               //��
    if (szBuff)                                                 //��
    {                                                           //��
        WCHAR szCurrentDirectory[MAX_PATH] = {0};               //��
        ::GetCurrentDirectoryW(MAX_PATH, szCurrentDirectory);   //��
        swprintf_s(                                             //��
            szBuff,                                             //��
            MAX_PATH,                                           //��
            szFile,                                             //��
            szCurrentDirectory                                  //��
            );                                                  //��
        return TRUE;                                            //��
    }                                                           //��
    return FALSE;                                               //��
}                                                               //��
inline BOOL CreateFullPathA(ACHAR* szBuff, ACHAR* szFile)       //��
{                                                               //��
    if (szBuff)                                                 //��
    {                                                           //��
        ACHAR szCurrentDirectory[MAX_PATH] = {0};               //��
        ::GetCurrentDirectoryA(MAX_PATH, szCurrentDirectory);   //��
        sprintf_s(                                              //��
            szBuff,                                             //��
            MAX_PATH,                                           //��
            szFile,                                             //��
            szCurrentDirectory                                  //��
            );                                                  //��
        return TRUE;                                            //��
    }                                                           //��
    return FALSE;                                               //��
}                                                               //��
                                                                //��
inline VOID ACharToWChar(                                       //��
    CONST ACHAR* szABuff,                                       //��
    WCHAR* szWBuff,                                             //��
    DWORD dwLen                                                 //��
    )                                                           //��
{                                                               //��
    // ���ַ�ת���ַ�                                           //��
    ::MultiByteToWideChar(                                      //��
        CP_ACP,                                                 //��
        0,                                                      //��
        szABuff,                                                //��
        (INT)strlen(szABuff) + 1,                               //��
        szWBuff,                                                //��
        dwLen                                                   //��
        );                                                      //��
}                                                               //��
inline VOID WCharToAChar(                                       //��
    CONST WCHAR* szWBuff,                                       //��
    ACHAR* szABuff,                                             //��
    DWORD dwLen                                                 //��
    )                                                           //��
{                                                               //��
    // ���ַ�ת���ַ�                                           //��
    ::WideCharToMultiByte(                                      //��
        CP_ACP,                                                 //��
        0,                                                      //��
        szWBuff,                                                //��
        (INT)wcslen(szWBuff) + 1,                               //��
        szABuff,                                                //��
        dwLen,                                                  //��
        NULL,                                                   //��
        NULL                                                    //��
        );                                                      //��
}                                                               //��
                                                                //��
#ifdef UNICODE                                                  //��
#   define ACharToTChar ACharToWChar                            //��
#   define WCharToTChar (w,t,l) _tcscpy_s(t,w,l);               //��
#   define TCharToAChar WCharToAChar                            //��
#   define TCharToWChar (t,w,l) _tcscpy_s(w,t,l);               //��
#else                                                           //��
#   define ACharToTChar (a,t,l) _tcscpy_s(t,a,l);               //��
#   define WCharToTChar WCharToAChar                            //��
#   define TCharToAChar (t,a,l) _tcscpy_s(a,t,l);               //��
#   define TCharToWChar ACharToWChar                            //��
#endif                                                          //��
//������������������������������������������������������������������

//�����������������빹����ζ�������ģ��,��������DWORD��������������
extern DWORD g_dwArrayHelperClassParameter;                     //��
extern MsLock g_ArrayHelperlock;                                //��
template <class T>                                              //��
class CMsApplyArrayHelper                                       //��
    :public T                                                   //��
{                                                               //��
public:                                                         //��
    CMsApplyArrayHelper(VOID)                                   //��
        : T(g_dwArrayHelperClassParameter){}                    //��
};                                                              //��
                                                                //��
template <class T>                                              //��
T* HelperNew(DWORD dwParameter, DWORD dwApplyCount)             //��
{                                                               //��
    AUTO_LOCK(g_ArrayHelperlock);                               //��
    g_dwArrayHelperClassParameter = dwParameter;                //��
    return (T*)(NEW CMsApplyArrayHelper<T>[dwApplyCount]);      //��
}                                                               //��
//������������������������������������������������������������������
// ����ģ��                                                     //��
#ifdef USE_MS_NET                                               //��
#   define USE_MS_LOG                                           //��
#   define USE_MS_THREAD                                        //��
#   define USE_MS_SOCKET                                        //��
#   define USE_MS_PACKET                                        //��
#   define USE_MS_PACKET_FACTORY                                //��
#   define USE_MS_IOCP                                          //��
#   define USE_MS_BUFFER                                        //��
#   define USE_MS_INDEX_POOL                                    //��
#   ifdef USE_MS_SOCKET                                         //��
        class CMsSocket;                                        //��
        typedef CMsSocket* lpCMsSocket;                         //��
        class CMsServerSocket;                                  //��
        typedef CMsServerSocket* lpCMsServerSocket;             //��
        class CMsClientSocket;                                  //��
        typedef CMsClientSocket* lpCMsClientSocket;             //��
#   endif                                                       //��
#   define SERVER_SOCKET_INDEX 0    // �������׽�������ֵ       //��
#endif                                                          //��
                                                                //��
#ifdef USE_MS_WINDOW                                            //��
#   define USE_MS_THREAD                                        //��
#endif                                                          //��
                                                                //��
#ifdef USE_MS_LUA                                               //��
#   define USE_MS_FIND_FILE                                     //��
#endif                                                          //��
//������������������������������������������������������������������


//����������������MFC ��ء�������������
// ��������                         //��
#include "MsAssertx.h"              //��
                                    //��
// GDI+ͼƬ                         //��
#ifdef USE_MS_GDI_PLUS_IMAGE        //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��MsGDI+")       //��
#endif                              //��
#include "MsGdiBuffImage.h"         //��
#include "MsGdiPlusImage.h"         //��
#endif                              //��
                                    //��
// ���ݿ�����                       //��
#ifdef USE_MS_DATA_BASE             //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��MsMFC���ݿ�")  //��
#endif                              //��
#include "MsAccess.h"               //��
#endif                              //��
                                    //��
// ��־����                         //��
#ifdef USE_MS_LOG                   //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms��־ϵͳ")   //��
#endif                              //��
#include "MsConsole.h"              //��
#include "MsLog.h"                  //��
#endif                              //��
                                    //��
// ��������                         //��
#ifdef USE_MS_INI                   //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms����ϵͳ")   //��
#endif                              //��
#include "MsIni.h"                  //��
#endif                              //��
                                    //��
// �̶߳���                         //��
#ifdef USE_MS_THREAD                //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms�̶߳���")   //��
#endif                              //��
#include "MsThread.h"               //��
#include "MsMsgThread.h"            //��
#endif                              //��
                                    //��
// �����ع���                       //��
#ifdef USE_MS_INDEX_POOL            //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms�����ؽṹ") //��
#endif                              //��
#include "MsPoolManager.h"          //��
#endif                              //��
                                    //��
// ���й���                         //��
#ifdef USE_MS_QUEUE_MANAGER         //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms���нṹ")   //��
#endif                              //��
#include "MsQueueManager.h"         //��
#endif                              //��
                                    //��
// ����ṹ                         //��
#ifdef USE_MS_PACKET                //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms���")       //��
#endif                              //��
#include "MsPacketDef.h"            //��
#endif                              //��
                                    //��
// ��������                         //��
#ifdef USE_MS_BUFFER                //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms������")     //��
#endif                              //��
#include "MsBuffer.h"               //��
#endif                              //��
                                    //��
// ��ɶ˿�                         //��
#ifdef USE_MS_IOCP                  //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms��ɶ˿�")   //��
#endif                              //��
#include "MsIocpManager.h"          //��
#endif                              //��
                                    //��
// �������                         //��
#ifdef USE_MS_PACKET_FACTORY        //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms���")       //��
#endif                              //��
#include "CMsPacketFactory.h"       //��
#endif                              //��
                                    //��
// �׽�����                         //��
#ifdef USE_MS_SOCKET                //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms�׽���")     //��
#endif                              //��
#include "MsSocket.h"               //��
#include "MsServerSocket.h"         //��
#include "MsClientSocket.h"         //��
#endif                              //��
                                    //��
// ��Դ�ļ�                         //��
#ifdef USE_MS_FILE_FROM_RES         //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms��Դ�ļ�")   //��
#endif                              //��
#include "MsFileFromRes.h"          //��
#endif                              //��
                                    //��
// �����ļ�                         //��
#ifdef USE_MS_FIND_FILE             //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms�����ļ�")   //��
#endif                              //��
#include "MsFindFileManager.h"      //��
#endif                              //��
                                    //��
// ���ݿ���                         //��
#ifdef USE_MS_SQLDB                 //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms���ݿ�ϵͳ") //��
#endif                              //��
#include <sqlext.h>                 //��
#include "MsSqlServerManager.h"     //��
#pragma comment(lib, "odbc32.lib")  //��
#endif                              //��
                                    //��
// ������                         //��
#ifdef USE_MS_TABLE                 //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms���ݿ�ϵͳ") //��
#endif                              //��
#include "MsAutoTableManager.h"     //��
#endif                              //��
                                    //��
// �ű�����                         //��
#ifdef USE_MS_LUA                   //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms�ű�ϵͳ")   //��
#endif                              //��
#include "MsLuaManager.h"           //��
#endif                              //��
                                    //��
// �ͻ������������                 //��
#ifdef USE_MS_CLIENT_NET            //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms�ͻ�������") //��
#endif                              //��
#include "MsClientNetManager.h"     //��
#endif                              //��
                                    //��
// ���������������                 //��
#ifdef USE_MS_SERVER_NET            //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms����������") //��
#endif                              //��
#include "MsServerNetManager.h"     //��
#endif                              //��
                                    //��
// ����������                       //��
#ifdef USE_MS_SOUND                 //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��Ms����������") //��
#endif                              //��
#include "MsSound.h"                //��
#endif                              //��
                                    //��
#ifdef USE_MS_2DENGINE              //��
#ifdef USE_COMPILER_MESSAGE         //��
#pragma message("ʹ��MsDirectX")    //��
#endif                              //��
#include "Ms2DEngineDef.h"          //��
#include "Ms2DEngine.h"             //��
#include "Ms2DInput.h"              //��
#include "Ms2DTextureManager.h"     //��
#include "Ms2DRender_DX9.h"         //��
#include "Ms2DEngine.h"             //��
#endif                              //��
//��������������������������������������

//��������������MFC ��ء�������������������
// ��ʹ��MFC                            //��
#ifndef USE_MFC                         //��
    // ����̨��                         //��
    #ifdef USE_MS_CONSOLE               //��
    #ifdef USE_COMPILER_MESSAGE         //��
    #pragma message("ʹ��Ms����̨ϵͳ") //��
    #endif                              //��
    #include "MsConsole.h"              //��
    #endif                              //��
    // ������                           //��
    #ifdef USE_MS_WINDOW                //��
    #ifdef USE_COMPILER_MESSAGE         //��
    #pragma message("ʹ��Ms����ϵͳ")   //��
    #endif                              //��
    #include "MsWinMain.h"              //��
    #endif                              //��
#endif                                  //��
//������������������������������������������

//����������������������������������������������������������������������
//ȫ�ֻ������ͳ�ʼ���߳̾��                                        //��
extern HANDLE   g_hBaseInitThread;                                  //��
                                                                    //��
//ȫ�ֻ��������ͷ��߳̾��                                          //��
extern HANDLE   g_hBaseReleaseThread;                               //��
                                                                    //��
//ȫ�ֻ������ͳ�ʼ���߳�ID                                          //��
extern DWORD    g_BaseInitthreadID;                                 //��
                                                                    //��
// GDI+������                                                     //��
extern ULONG_PTR g_lToken;                                          //��
                                                                    //��
BOOL InitRand(VOID* /*Parameter*/);                                 //��
                                                                    //��
extern BOOL g_IsMsBaseInit;                                         //��
                                                                    //��
// �궨���ʼ�����ȴ��߳̽���                                       //��
VOID MSCALL InitMsBase(                                             //��
    TCHAR* szAssertFileName,                                        //��
    BOOL bConsole,                                                  //��
    RUNCALLBACK pRunCallBack,                                       //��
    LPTSTR szClassName,                                             //��
    LPTSTR szWndName,                                               //��
    HOOKWNDPROC HookWndProc,                                        //��
    WNDPROC WndProc,                                                //��
    PTHREAD_START_ROUTINE ThcreadProc                               //��
    );                                                              //��
VOID MSCALL ReleaseMsBase(LPVOID lpNeedKnowIsFunForUse);            //��
class CMsBaseRun                                                    //��
{                                                                   //��
public:                                                             //��
//����������������������������������������������������������������������
    CMsBaseRun(                                                     //��
        TCHAR* szAssertFileName,                                    //��
        RUNCALLBACK pRunCallBack                                    //��
        )                                                           //��
    {                                                               //��
        InitMsBase(                                                 //��
            szAssertFileName,   // TCHAR* szAssertFileName          //��
            TRUE,               // BOOL bConsole                    //��
            pRunCallBack,       // RUNCALLBACK pRunCallBack         //��
            NULL,               // LPTSTR szClassName               //��
            NULL,               // LPTSTR szWndName                 //��
            NULL,               // HOOKWNDPROC HookWndProc          //��
            NULL,               // WNDPROC WndProc                  //��
            NULL                // PTHREAD_START_ROUTINE ThcreadProc//��
            );                                                      //��
    }                                                               //��
//����������������������������������������������������������������������
    CMsBaseRun(                                                     //��
        TCHAR* szAssertFileName,                                    //��
        LPTSTR szClassName,                                         //��
        LPTSTR szWndName,                                           //��
        HOOKWNDPROC HookWndProc,                                    //��
        WNDPROC WndProc,                                            //��
        PTHREAD_START_ROUTINE ThcreadProc                           //��
        )                                                           //��
    {                                                               //��
        InitMsBase(                                                 //��
            szAssertFileName,   // TCHAR* szAssertFileName          //��
            FALSE,              // BOOL bConsole                    //��
            NULL,               // RUNCALLBACK pRunCallBack         //��
            szClassName,        // LPTSTR szClassName               //��
            szWndName,          // LPTSTR szWndName                 //��
            HookWndProc,        // HOOKWNDPROC HookWndProc          //��
            WndProc,            // WNDPROC WndProc                  //��
            ThcreadProc         // PTHREAD_START_ROUTINE ThcreadProc//��
            );                                                      //��
    }                                                               //��
//����������������������������������������������������������������������
    CMsBaseRun(TCHAR* szAssertFileName)                             //��
    {                                                               //��
        InitMsBase(                                                 //��
            szAssertFileName,   // TCHAR* szAssertFileName          //��
            FALSE,              // BOOL bConsole                    //��
            NULL,               // RUNCALLBACK pRunCallBack         //��
            NULL,               // LPTSTR szClassName               //��
            NULL,               // LPTSTR szWndName                 //��
            NULL,               // HOOKWNDPROC HookWndProc          //��
            NULL,               // WNDPROC WndProc                  //��
            NULL                // PTHREAD_START_ROUTINE ThcreadProc//��
            );                                                      //��
    }                                                               //��
//����������������������������������������������������������������������
    ~CMsBaseRun(VOID){ReleaseMsBase(NULL);}                         //��
private:                                                            //��
    CMsBaseRun(VOID){};                                             //��
};                                                                  //��
//����������������������������������������������������������������������

#if     (_OPENMP == 0)
#   error  �޷�ʶ��������汾, ��򿪹��� -> C/C++ -> ���� -> OpenMP ֧�� -> ��
#elif   (_OPENMP == 200805) && !defined(_INTEL)
#   error  Intel����������ʹ��Intel����ѡ����б���!
#elif   (_OPENMP == 200203) && defined(_INTEL)
#   error  Visual C++����������ʹ��VC����ѡ����б���!
#endif

//������������������������������������������������������������������������������
#ifdef _INTEL                                                               //��
#pragma warning(disable : 1684)                                             //��
#pragma warning(disable : 111)                                              //��
#else                                                                       //��
#endif                                                                      //��
                                                                            //��
// Windows Lib                                                              //��
#pragma comment(lib, "Psapi.lib")                                           //��
                                                                            //��
#if defined(WIN64)                                                          //��
#   ifdef _DEBUG                                                            //��
#       ifdef _INTEL                                                        //��
#           pragma comment(lib, "Lib_ExotericCode_Intel_Debug_x64.lib")     //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC_Intel_Debug_x64.lib")        //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode_Intel_Debug_x64.lib")       //��
#           endif                                                           //��
#       else                                                                //��
#           pragma comment(lib, "Lib_ExotericCode__VC_Debug_x64.lib")       //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC__VC_Debug_x64.lib")          //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode__VC_Debug_x64.lib")         //��
#           endif                                                           //��
#       endif                                                               //��
#   else                                                                    //��
#       ifdef _INTEL                                                        //��
#           pragma comment(lib, "Lib_ExotericCode_Intel_Release_x64.lib")   //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC_Intel_Release_x64.lib")      //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode_Intel_Release_x64.lib")     //��
#           endif                                                           //��
#       else                                                                //��
#           pragma comment(lib, "Lib_ExotericCode__VC_Release_x64.lib")     //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC__VC_Release_x64.lib")        //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode__VC_Release_x64.lib")       //��
#           endif                                                           //��
#       endif                                                               //��
#   endif                                                                   //��
#elif defined(WIN32)                                                        //��
#   ifdef _DEBUG                                                            //��
#       ifdef _INTEL                                                        //��
#           pragma comment(lib, "Lib_ExotericCode_Intel_Debug_Win32.lib")   //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC_Intel_Debug_Win32.lib")      //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode_Intel_Debug_Win32.lib")     //��
#           endif                                                           //��
#       else                                                                //��
#           pragma comment(lib, "Lib_ExotericCode__VC_Debug_Win32.lib")     //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC__VC_Debug_Win32.lib")        //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode__VC_Debug_Win32.lib")       //��
#           endif                                                           //��
#       endif                                                               //��
#   else                                                                    //��
#       ifdef _INTEL                                                        //��
#           pragma comment(lib, "Lib_ExotericCode_Intel_Release_Win32.lib") //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC_Intel_Release_Win32.lib")    //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode_Intel_Release_Win32.lib")   //��
#           endif                                                           //��
#       else                                                                //��
#           pragma comment(lib, "Lib_ExotericCode__VC_Release_Win32.lib")   //��
#           ifdef USE_MFC                                                   //��
#               pragma comment(lib, "Lib_MsMFC__VC_Release_Win32.lib")      //��
#           else                                                            //��
#               pragma comment(lib, "Lib_MsCode__VC_Release_Win32.lib")     //��
#           endif                                                           //��
#       endif                                                               //��
#   endif                                                                   //��
#endif                                                                      //��
//������������������������������������������������������������������������������

//����������������������
#endif//#pragma once//��
//����������������������
