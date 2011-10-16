// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#pragma warning(disable:4996)

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <shlwapi.h>
#include <stdlib.h>
#include <wchar.h>

using namespace std;



#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>
using namespace std;

/*
//����â����Ʒϵͳ����Ҫ������������
//�ýṹ����
struct tagModify {
	BYTE	lFixID;		// �����ĸ�ֵ��0:Value1 1:Value2��
	long	dwMin;		// ������Сֵ
	long	dwMax;		// �������ֵ
	WORD	wOdds;		// ����(1/10000);	
};*/

struct  tagAddAttr{
	WORD	wType;			// ��������
	bool	bEnable;		// �Ƿ���Ч(Ϊ�����������Ԥ��)
	bool	bHide;			// �Ƿ�����������
	long	lValue1;		// ��һ��ֵ
	long	lValue2;		// �ڶ���ֵ
	//���ټ�����ּ�����
	//�༭��ӵ�����100%����
	//WORD	wEnableOdds;    // ���ּ���
	//vector<tagModify> vecModify;
};

struct tagSuitAttr
{
	WORD wType;
	long lValue1;
	long lValue2;
	WORD wSuitNum;
};

//add by wang-xx 2009-6-30 begin
//��������
struct tagArouseAttr
{
	WORD wType;
	long lValue1;
	long lValue2;
	WORD wArouseCnt;
};
//add by wang-xx 2009-6-30 end

struct tagGoodAttr {
	DWORD	dwIndex;		//	���
	string  strOrigName;	//	ԭʼ��   < 32
	string	strName;		//	�������� < 32

	bool	bSave;			//	�Ƿ���Ҫ�����ݿ�
	DWORD	dwValue;		//	�۸�
	DWORD   dwSilverValue;	//  ���Ҽ۸�
	//BYTE	lX_Size;		//	��ƷX�����С
	//BYTE	lY_Size;		//	��Ʒy�����С
	DWORD	dwType;			//	��Ʒ����
	//DWORD	dwWeight;		//	����

	vector<tagAddAttr> vecAddAttr;
	vector<tagSuitAttr> vecSuitAttr;

	//add by wang-xx 2009-6-30 begin
	vector<tagArouseAttr> vecArouseAttr;
	//add by wang-xx 2009-6-30 end

	// �ͻ���ר������
	DWORD	dwIconId;		//	����ͼ��ID
	DWORD	dwGroundId;		//	��غ�ͼ��ID
	DWORD	dwEquipID;		//	��װͼ��ID
	DWORD	dwSound;		//	ʰȡ����
	DWORD	dwSoundID1;		//	�Ӷ�/���˺�����
	DWORD	dwSoundID2;		//	���⹥��/��������
	bool	bSoundSwitch;	//	����ʱ�Ƿ����
	DWORD	dwWeaponActType;//	������Ӧ�Ķ�������
	string	strContent;		//	�������� < 64
};