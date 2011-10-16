/**
============================================================
*  Copyright (c) 2010
*  All rights reserved
*  �ļ����ƣ�TestReporter.cpp
*  ���ܣ����Ա�����
*  ��ǰ�汾��1.0.0
*  ���ߣ�    ������
*  ������ڣ�2010.11.8
*  ȡ���汾��---
*  ԭ���ߣ�	 ������
*  ������ڣ�2010.11.8
*
============================================================
*/
#include "stdafx.h"
#include "TestReporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

string CTestReporter::m_strConctIp("");					//�˴β������ӷ�����IP
long   CTestReporter::m_lConctTotlNum = 0;				//�˴β�����������
string CTestReporter::m_strTestTypeNodeNum("");			//�˴β������ͽڵ����
string CTestReporter::m_strTestTypeName("");			//�˴β������ͽڵ�����
string CTestReporter::m_strProjName("");				//�˴β�����Ŀ����
string CTestReporter::m_strStartTestTime("");			//���Կ�ʼʱ��
string CTestReporter::m_strLaseTestTime("");				//���Խ���ʱ��
string CTestReporter::m_strStartCDKEY("");				//���Կ�ʼ�˺�
string CTestReporter::m_strEndCDKEY("");				//���Խ����˺�
long CTestReporter::m_lMaxConctNum = 0;					//�˴β���ͬʱ��¼��ֵ
long CTestReporter::m_lMinConctNum = 0;					//�˴β���ͬʱ��¼��ֵ
long CTestReporter::m_lKickedByServerNum = 0;	        //�˴β��Է������߳���е��������
long CTestReporter::m_lPerHourConctNum = 0;				//�˴β���ÿСʱ��ƽ����������
bool CTestReporter::m_bIsCanRun = false;				//�˴β��Ե�һ�νű��Ƿ���ִ������������
long CTestReporter::m_lTotalRuntime = 0;			    //�˴β��Ի�е����������ʱ��
long CTestReporter::m_lMsgInterchangeNum = 0;		    //�˴β��Ի�е����������������ݽ�����
long CTestReporter::m_lMaxConctNumSysTest = 0;			//�˴β��Ի�е�������������
bool CTestReporter::m_bIsRadHeadRadData = false;		//�˴β��Ի�е���Ƿ�ʹ�����MsgHead,���MsgData
bool CTestReporter::m_bIsFixedHeadFixedData = false;	//�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,�̶�MsgData
bool CTestReporter::m_bIsFixedHeadRadData = false;		//�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,���MsgData
bool CTestReporter::m_bIsRadHeadFixeData = false;		//�˴β��Ի�е���Ƿ�ʹ�����MsgHead,�̶�MsgData
bool CTestReporter::m_bIsJumpLogicDrive = false;		//�˴β����Ƿ������߼��жϳɹ�


CTestReporter::CTestReporter()
{
	m_pDlg = NULL;
}
CTestReporter::~CTestReporter()
{

}


/*
=========================================================================
��������:TestReporterLogic��ʼ������  By: LGR
=========================================================================
*/
bool CTestReporter::Init(void* p)
{
	m_pDlg = p;
	return true;
}


/*
=========================================================================
��������:TestReporterLogic ��̬ȫ�ֶ��󴴽�����  By: LGR
=========================================================================
*/
CTestReporter*	CTestReporter::GetInstance()
{
	static CTestReporter*	pInstance = NULL;
	if(NULL == pInstance)
	{
		pInstance= new CTestReporter();
	}
	return pInstance;
}


/*
=========================================================================
��������:TestReporter ���ö����ͷ�  By: LGR
=========================================================================
*/
bool CTestReporter::Release()
{
	CTestReporter* pInstance = GetInstance();
	SAFE_DELETE(pInstance);
	return true;
}

/*
=========================================================================
��������:TestReporter �õ����Խ������ַ���  By: LGR
=========================================================================
*/
string	CTestReporter::GetTestResultInfo()
{
	char _out[1024] = "";
	sprintf(_out,"�˴β������ӷ�����IP:%s\r\n�˴β�����������: %d\r\n�˴β������ͽڵ�: %s\r\n�˴β������ͽڵ�����: %s\r\n�˴β�����Ŀ����: %s\r\n���Կ�ʼʱ��: %s\r\n���Գ���ʱ��: %s\r\n���Կ�ʼ�˺�: %s\r\n���Խ����˺�: %s\r\n�˴β���ͬʱ��¼��ֵ: %d\r\n�˴β���ͬʱ��¼��ֵ: %d\r\n�˴β��Է������߳���е��������: %d\r\n�˴β���ÿСʱ��ƽ����������: %d\r\n�˴β��Ե�һ�νű��Ƿ���ִ������������: %d\r\n�˴β��Ի�е����������ʱ��: %d\r\n�˴β��Ի�е����������������ݽ�����: %d\r\n�˴β��Ի�е�������������:  %d\r\n�˴β��Ի�е���Ƿ�ʹ�����MsgHead,���MsgData:  %d\r\n�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,�̶�MsgData: %d\r\n�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,���MsgData: %d\r\n�˴β��Ի�е���Ƿ�ʹ�����MsgHead,�̶�MsgData: %d\r\n�˴β����Ƿ������߼��жϳɹ�: %d\r\n",m_strConctIp.c_str(),m_lConctTotlNum,m_strTestTypeNodeNum.c_str(),m_strTestTypeName.c_str(),m_strProjName.c_str(),m_strStartTestTime.c_str(),m_strLaseTestTime.c_str(),m_strStartCDKEY.c_str(),m_strEndCDKEY.c_str(),m_lMaxConctNum,m_lMinConctNum,m_lKickedByServerNum,m_lPerHourConctNum,m_bIsCanRun,m_lTotalRuntime,m_lMsgInterchangeNum,m_lMaxConctNumSysTest,m_bIsRadHeadRadData,m_bIsFixedHeadFixedData,m_bIsFixedHeadRadData,m_bIsRadHeadFixeData,m_bIsJumpLogicDrive);
	return _out;
}