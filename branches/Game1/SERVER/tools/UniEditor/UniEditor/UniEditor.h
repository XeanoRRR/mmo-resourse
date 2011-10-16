// UniEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <fstream>
#include <string>

#define			MAX_TIME	500
using namespace std;

const string strSharp		= _T("#\t");
const string strEnd			= _T("<end>");					// ������־
const string strSep			= _T("\t");						// �ָ���
const string strLine		= _T("----------------------------------------------------------------------------\n");
const string strMonsterInfoHead = _T("	����	����	��ʶ	�쵼	����	·����ʶ	�ű�	�Ի��ű�		\n");
const string strCollInfoHead = _T("	����		����		\n");
const string strMonsterHead = _T("	���	���귶Χ(��X1\\Y1\\��X2\\Y2)	����	ʱ��	��ʼʱ	����\n");
const string strNPCHead		= _T("	���귶Χ(��X1\\Y1\\��X1\\Y1)	ԭʼ����			ͼƬ	����	����	�ű�\n");
const string strLogFile		= _T("");
const string strXMLHead		=_T("<?xml version=\"1.0\" encoding=\"gb2312\" standalone=\"no\" ?>\n <RoadPoint>\n");
const string strXMLTail		=_T("\n</Region>\n</RoadPoint>");
// CUniEditorApp:
// �йش����ʵ��,����� UniEditor.cpp
//

class CUniEditorApp : public CWinApp
{
public:
	CUniEditorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
public:
	static BOOL ReadTo(const string &strReadTo, ifstream &ifStream);

	DECLARE_MESSAGE_MAP()
};

extern CUniEditorApp theApp;