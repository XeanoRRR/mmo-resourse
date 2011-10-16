/*
=========================================================================
��ע����δʹ�ã�����ȡ����2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�LinkManDlg.cpp
*  ���ܣ����Ѳ��ԶԻ���
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�MartySa
*  ������ڣ� 2009.4.24
*
============================================================
*/

#include "stdafx.h"
#include "GameTest.h"
#include "LinkManDlg.h"
#include "../Script/ScriptSys.h"
#include "GameTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CLinkManDlg, CDialog)

CLinkManDlg::CLinkManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkManDlg::IDD, pParent)
	, m_lRegionID(60101)
	, m_lPosX(301)
	, m_lPosY(254)
	, m_lStartCdKey(1900)
	, m_lEndCdKey(1999)
	, m_lSendMsgTimer(800)
	, m_lSeverSub(5008)
	, m_lMoveRrob(1)
	, m_lDelProb(1)
	, m_lExitProb(25)
	, m_isCanSendAdd(false)				//�Ƿ��������
	, m_isCanResponseAdd(false)			//�Ƿ�ظ�����
	, m_isCanMoveLink(false)			//�Ƿ��ƶ�����
	, m_isCanDelLink(false)				//�Ƿ�ɾ������
	, m_isCanExit(false)				//�Ƿ�������
	, m_isCanPtLogin(false)				//�Ƿ�ʢ���½��ʽ
{

}

CLinkManDlg::~CLinkManDlg()
{
}

void CLinkManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_REGION, m_lRegionID);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_POSX, m_lPosX);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_POSY, m_lPosY);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_StartCdKey, m_lStartCdKey);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_EndCdkey, m_lEndCdKey);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_MsgTimer, m_lSendMsgTimer);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_SeverSub, m_lSeverSub);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_MoveProbability, m_lMoveRrob);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_DelProbability, m_lDelProb);
	DDX_Text(pDX, IDC_EDIT_LINKMAN_ExitProbability, m_lExitProb);
	DDX_Control(pDX, IDC_COMBO_LINKMAN_SeverIP, m_comboServerIP);
}


BEGIN_MESSAGE_MAP(CLinkManDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_LINKMAN_SendAdd, &CLinkManDlg::OnBnClickedCheckLinkmanSendadd)
	ON_BN_CLICKED(IDC_CHECK_LINKMAN_ResponseAdd, &CLinkManDlg::OnBnClickedCheckLinkmanResponseadd)
	ON_BN_CLICKED(IDC_CHECK_LINKMAN_SendMove, &CLinkManDlg::OnBnClickedCheckLinkmanSendmove)
	ON_BN_CLICKED(IDC_CHECK_LINKMAN_Del, &CLinkManDlg::OnBnClickedCheckLinkmanDel)
	ON_BN_CLICKED(IDC_CHECK_LINKMAN_ExitClient, &CLinkManDlg::OnBnClickedCheckLinkmanExitclient)
	ON_BN_CLICKED(IDC_BUTTON_LINKMAN_START, &CLinkManDlg::OnBnClickedButtonLinkmanStart)
	ON_BN_CLICKED(IDC_CHECK_LINKMAN_PT_LOGIN, &CLinkManDlg::OnBnClickedCheckLinkmanPtLogin)
END_MESSAGE_MAP()

BOOL CLinkManDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	return TRUE;
}

// CLinkManDlg ��Ϣ�������
void CLinkManDlg::OnBnClickedCheckLinkmanSendadd()
{
	m_isCanSendAdd = ((CButton*)GetDlgItem(IDC_CHECK_LINKMAN_SendAdd))->GetCheck() ? true : false ;
}

void CLinkManDlg::OnBnClickedCheckLinkmanResponseadd()
{
	m_isCanResponseAdd = ((CButton*)GetDlgItem(IDC_CHECK_LINKMAN_ResponseAdd))->GetCheck() ? true : false ;
}

void CLinkManDlg::OnBnClickedCheckLinkmanSendmove()
{
	m_isCanMoveLink = ((CButton*)GetDlgItem(IDC_CHECK_LINKMAN_SendMove))->GetCheck() ? true : false ;
}

void CLinkManDlg::OnBnClickedCheckLinkmanDel()
{
	m_isCanDelLink = ((CButton*)GetDlgItem(IDC_CHECK_LINKMAN_Del))->GetCheck() ? true : false ;
}

void CLinkManDlg::OnBnClickedCheckLinkmanExitclient()
{
	m_isCanExit = ((CButton*)GetDlgItem(IDC_CHECK_LINKMAN_ExitClient))->GetCheck() ? true : false ;
}


//ʢ���½��ʽCheck
void CLinkManDlg::OnBnClickedCheckLinkmanPtLogin()
{
	m_isCanPtLogin = ((CButton*)GetDlgItem(IDC_CHECK_LINKMAN_PT_LOGIN))->GetCheck() ? true : false ;
	if ( m_isCanPtLogin )
	{
		((CButton*)GetDlgItem(IDC_STATIC_LINKMAN_strEndCDkey))->ShowWindow(false);	
		((CButton*)GetDlgItem(IDC_EDIT_LINKMAN_EndCdkey))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_LINKMAN_strStartCDkey))->SetWindowText("����������:");
	}
	else
	{
		((CButton*)GetDlgItem(IDC_STATIC_LINKMAN_strEndCDkey))->ShowWindow(true);	
		((CButton*)GetDlgItem(IDC_EDIT_LINKMAN_EndCdkey))->ShowWindow(true);
		((CButton*)GetDlgItem(IDC_STATIC_LINKMAN_strStartCDkey))->SetWindowText("��ʼ�˺�: GM");
	}
}

void CLinkManDlg::OnBnClickedButtonLinkmanStart()
{
	UpdateData(TRUE);
	char strScriptName[256] = " ";

	if ( !m_isCanPtLogin )		//SiMo��½��ʽ
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
		

		sprintf(strScriptName,"LinkManLoginCDKey(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
													m_lStartCdKey,				//��ʼ�ʺ�
													m_lEndCdKey,				//�����ʺ�
													10,							//��������
													m_lSendMsgTimer,			//��Ϣ���
													m_lRegionID,				//��ʼ����Id									
													m_lPosX,					//����X									
													m_lPosY,					//����Y									
													m_lSeverSub,				//�������汾	
													m_lMoveRrob,				//�ƶ�����
													m_lDelProb,					//ɾ������
													m_lExitProb,				//�˳�����
													m_isCanSendAdd,				//�Ƿ��������
													m_isCanResponseAdd,			//�Ƿ�ظ�����
													m_isCanMoveLink,			//�Ƿ��ƶ�����
													m_isCanDelLink,				//�Ƿ�ɾ������
													m_isCanExit				//�Ƿ�������
													);
	}
	else
	{
		if ( m_lStartCdKey < 0 )
		{	
			::MessageBox(NULL, "�������ʼ�ʺ�����С��0,����������", "ERRO!", MB_OK);
			return ;
		}
		//��������EndCdkey
		sprintf(strScriptName,"LinkManPtLoginCDKey(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
			m_lStartCdKey,				//��ʼ�ʺ�
			//m_lEndCdKey,				//�����ʺ�
			10,							//��������
			m_lSendMsgTimer,			//��Ϣ���
			m_lRegionID,				//��ʼ����Id									
			m_lPosX,					//����X									
			m_lPosY,					//����Y									
			m_lSeverSub,				//�������汾	
			m_lMoveRrob,				//�ƶ�����
			m_lDelProb,					//ɾ������
			m_lExitProb,				//�˳�����
			m_isCanSendAdd,				//�Ƿ��������
			m_isCanResponseAdd,			//�Ƿ�ظ�����
			m_isCanMoveLink,			//�Ƿ��ƶ�����
			m_isCanDelLink,				//�Ƿ�ɾ������
			m_isCanExit				//�Ƿ�������
			);
	}
	CScriptSys::getInstance()->PerformString(strScriptName);
}
