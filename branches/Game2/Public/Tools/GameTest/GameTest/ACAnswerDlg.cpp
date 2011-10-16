/*
=========================================================================
��ע���ù�����ȡ��������ȡ����2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�ACAnswerDlg.h
*  ���ܣ���������ԶԻ������
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�
*
============================================================
*/

#include "stdafx.h"
#include "GameTest.h"
#include "ACAnswerDlg.h"
#include "../Script/ScriptSys.h"
#include "GameTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CACAnswer, CDialog)

CACAnswer::CACAnswer(CWnd* pParent)
	: CDialog(CACAnswer::IDD, pParent)
	, m_lRegionID(20100)
	, m_lPosX(0)
	, m_lPosY(0)
	, m_lStartCdKey(1900)
	, m_lEndCdKey(1999)
	, m_lSendMsgTimer(800)
	, m_lSeverSub(5008)
	, m_bIsCanAnswer(TRUE)
	, m_bIsCanSafeTest(FALSE)
	, m_bIsCopyMsg(FALSE)
	, m_bIsCopySnedMsg(FALSE)
	, m_bIsExitClient(FALSE)
	, m_bIsCanPtLogin(FALSE)
{

}

CACAnswer::~CACAnswer()
{
}

void CACAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACC_REGION, m_lRegionID);
	DDX_Text(pDX, IDC_EDIT_ACC_POSX, m_lPosX);
	DDX_Text(pDX, IDC_EDIT_ACC_POSY, m_lPosY);
	DDX_Text(pDX, IDC_EDIT_ACC_StartCdKey, m_lStartCdKey);
	DDX_Text(pDX, IDC_EDIT_ACC_EndCdkey, m_lEndCdKey);
	DDX_Text(pDX, IDC_EDIT_ACC_MsgTimer, m_lSendMsgTimer);
	DDX_Text(pDX, IDC_EDIT_ACC_SeverSub, m_lSeverSub);
	DDX_Check(pDX, IDC_ACC_CHECK_Answer, m_bIsCanAnswer);
	DDX_Check(pDX, IDC_ACC_CHECK_SAFE, m_bIsCanSafeTest);
	DDX_Check(pDX, IDC_ACC_CHECK_CopyMsg, m_bIsCopyMsg);
	DDX_Check(pDX, IDC_ACC_CHECK_CopySend, m_bIsCopySnedMsg);
	DDX_Check(pDX, IDC_ACC_CHECK_ExitClient, m_bIsExitClient);
}


BEGIN_MESSAGE_MAP(CACAnswer, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ACC_START, &CACAnswer::OnBnClickedButtonAccStart)
	ON_BN_CLICKED(IDC_ACC_CHECK_Answer, &CACAnswer::OnBnClickedAccCheckAnswer)
	ON_BN_CLICKED(IDC_ACC_CHECK_SAFE, &CACAnswer::OnBnClickedAccCheckSafe)
	ON_BN_CLICKED(IDC_ACC_CHECK_CopyMsg, &CACAnswer::OnBnClickedAccCheckCopymsg)
	ON_BN_CLICKED(IDC_ACC_CHECK_CopySend, &CACAnswer::OnBnClickedAccCheckCopysend)
	ON_BN_CLICKED(IDC_ACC_CHECK_ExitClient, &CACAnswer::OnBnClickedAccCheckExitclient)
	ON_BN_CLICKED(IDC_CHECK_ACA_PT_LOGIN, &CACAnswer::OnBnClickedCheckAcaPtLogin)
END_MESSAGE_MAP()


void CACAnswer::OnBnClickedButtonAccStart()
{
	UpdateData(TRUE);
	char strScriptName[256] = " ";
	
	if ( !m_bIsCanPtLogin )			// SiMo��ʽ��½
	{
		if ( m_lStartCdKey > m_lEndCdKey )
		{	
			::MessageBox(NULL, "�������ʼ�ʺ�<<С��>>�����ʺ�,�����������ʺ�","ERRO!", MB_OK);
			return ;
		}
		if ( m_lStartCdKey < 1000 )
		{
			::MessageBox(NULL, "�������ʼ�ʺ�<<����>> 1000 ","ERRO!", MB_OK);
			return ; 
		}
		if ( m_lStartCdKey == m_lEndCdKey )
		{
			::MessageBox(NULL, "�������ʼ�ʺŵ��ڽ����ʺ�,Ϊһ���ʺ�","OK!", MB_OK);
		}
		

		sprintf(strScriptName,"ACCAnswerLoginCDKey(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			m_lStartCdKey,				//��ʼ�ʺ�
			m_lEndCdKey,				//�����ʺ�
			12,							//��������
			m_lSendMsgTimer,			//��Ϣ���
			m_lRegionID,				//��ʼ����Id									
			m_lPosX,					//����X									
			m_lPosY,					//����Y									
			m_lSeverSub,				//�������汾	
			m_bIsCanAnswer,
										// �Ƿ񼤻ȫ����
			m_bIsCanSafeTest,
										// �Ƿ���������Ϣ
			m_bIsCopyMsg,
										// �Ƿ�����ϢSend�Ǹ���
			m_bIsCopySnedMsg,
										// �Ƿ�������
			m_bIsExitClient	
			);
	}
	else
	{
		if ( m_lStartCdKey < 0 )
		{	
			::MessageBox(NULL, "�������ʼ�ʺ�����С��0,����������", "ERRO!", MB_OK);
			return ;
		}

		sprintf(strScriptName,"ACCAnswerPtLoginCDKey(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			m_lStartCdKey,				//��ʼ�ʺ�
			//m_lEndCdKey,				//�����ʺ�
			12,							//��������
			m_lSendMsgTimer,			//��Ϣ���
			m_lRegionID,				//��ʼ����Id									
			m_lPosX,					//����X									
			m_lPosY,					//����Y									
			m_lSeverSub,				//�������汾	
			m_bIsCanAnswer,
			// �Ƿ񼤻ȫ����
			m_bIsCanSafeTest,
			// �Ƿ���������Ϣ
			m_bIsCopyMsg,
			// �Ƿ�����ϢSend�Ǹ���
			m_bIsCopySnedMsg,
			// �Ƿ�������
			m_bIsExitClient	
			);
	}

	CScriptSys::getInstance()->PerformString(strScriptName);

}

void CACAnswer::OnBnClickedAccCheckAnswer()
{
	m_bIsCanAnswer = ((CButton*)GetDlgItem(IDC_ACC_CHECK_Answer))->GetCheck() ? true : false ;
}

void CACAnswer::OnBnClickedAccCheckSafe()
{
	m_bIsCanSafeTest = ((CButton*)GetDlgItem(IDC_ACC_CHECK_SAFE))->GetCheck() ? true : false ;
}

void CACAnswer::OnBnClickedAccCheckCopymsg()
{
	m_bIsCopyMsg = ((CButton*)GetDlgItem(IDC_ACC_CHECK_CopyMsg))->GetCheck() ? true : false ;
}

void CACAnswer::OnBnClickedAccCheckCopysend()
{
	m_bIsCopySnedMsg = ((CButton*)GetDlgItem(IDC_ACC_CHECK_CopySend))->GetCheck() ? true : false ;
}

void CACAnswer::OnBnClickedAccCheckExitclient()
{
	m_bIsExitClient = ((CButton*)GetDlgItem(IDC_ACC_CHECK_ExitClient))->GetCheck() ? true : false ;
}

void CACAnswer::OnBnClickedCheckAcaPtLogin()
{
	m_bIsCanPtLogin = ((CButton*)GetDlgItem(IDC_CHECK_ACA_PT_LOGIN))->GetCheck() ? true : false ;
	if ( m_bIsCanPtLogin )
	{
		((CButton*)GetDlgItem(IDC_STATIC_ACC_strEndCDkey))->ShowWindow(false);	
		((CButton*)GetDlgItem(IDC_EDIT_ACC_EndCdkey))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_ACC_strStartCDkey))->SetWindowText("����������:");
	}
	else
	{
		((CButton*)GetDlgItem(IDC_STATIC_ACC_strEndCDkey))->ShowWindow(true);	
		((CButton*)GetDlgItem(IDC_EDIT_ACC_EndCdkey))->ShowWindow(true);
		((CButton*)GetDlgItem(IDC_STATIC_ACC_strStartCDkey))->SetWindowText("��ʼ�˺�: GM");
	}
}
