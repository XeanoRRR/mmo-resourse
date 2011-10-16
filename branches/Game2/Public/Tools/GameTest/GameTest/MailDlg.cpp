/**
=========================================================================
��ע����δʹ�ã�����ȡ����2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�MailDlg.cpp
*  ���ܣ��ʼ����ԶԻ���
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
#include "MailDlg.h"
#include "../Script/ScriptSys.h"
#include "GameTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMailDlg, CDialog)

CMailDlg::CMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailDlg::IDD, pParent)
	, m_lStartRegionID(60101)
	, m_lPosX(301)
	, m_lPosY(254)
	, m_lStartCdKey(1900)
	, m_lEndCdKey(1999)
	, m_lMsgSendTimer(800)
	, m_cstrGoodsName(_T("����趨"))
	, m_lSendMoney(0)
	, m_lMailType(-1)
	, m_lGoodsNum(-1)
	, m_cstrSendName(_T("����趨"))
	, m_lSendRepeatNum(0)
	, m_lRejectMailProb(0)
	, m_lExitClientPro(25)
	, m_lDeleteMailProb(0)
	, m_lSeverSub(5008)
	, m_lSendMsgRepeatNum(0)
	, m_isCanSendMail(false)			//�Ƿ��ܷ���
	, m_isCanRepeatSendName(false)		//�Ƿ��ظ����͸�һ�����
	, m_isCanRecMail(false)				//�Ƿ�������
	, m_isCanRejectMail(false)			//�Ƿ�����
	, m_isCanDeleteMail(false)			//�Ƿ�ɾ��
	, m_isCanGetGoods(false)			//�Ƿ�ȡ��Ʒ
	, m_isCanAgreePay(false)			//�Ƿ�ͬ�⸶��
	, m_isCanGetMoney(false)			//�Ƿ�ȡ��Ǯ
	, m_isCanExitClient(false)			//�Ƿ�������
	, m_isCanSendMsgRepeat(false)		//�Ƿ��ظ�����һ����Ϣ
	, m_isCanPtLogin(false)				//�Ƿ�ʢ���½��ʽ
{

}

CMailDlg::~CMailDlg()
{
}

void CMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAIL_REGION, m_lStartRegionID);
	DDX_Text(pDX, IDC_EDIT_MAIL_POSX, m_lPosX);
	DDX_Text(pDX, IDC_EDIT_MAIL_POSY, m_lPosY);
	DDX_Text(pDX, IDC_EDIT_MAIL_StartCdKey, m_lStartCdKey);
	DDX_Text(pDX, IDC_EDIT_MAIL_EndCdkey, m_lEndCdKey);
	DDX_Text(pDX, IDC_EDIT_MAIL_MsgTimer, m_lMsgSendTimer);	
	DDX_Text(pDX, IDC_EDIT_MAIL_SendDoodsName, m_cstrGoodsName);
	DDX_Text(pDX, IDC_EDIT_MAIL_SendMoney, m_lSendMoney);
	DDX_Text(pDX, IDC_EDIT_MAIL_SendType, m_lMailType);
	DDX_Text(pDX, IDC_EDIT_MAIL_SendGoodsNum, m_lGoodsNum);
	DDX_Text(pDX, IDC_EDIT_MAIL_SendName, m_cstrSendName);
	DDX_Text(pDX, IDC_EDIT_MAIL_SendRepeatNum, m_lSendRepeatNum);
	DDX_Text(pDX, IDC_EDIT_MAIL_RejectProbability, m_lRejectMailProb);
	DDX_Text(pDX, IDC_EDIT_MAIL_ExitProbability, m_lExitClientPro);
	DDX_Text(pDX, IDC_EDIT_MAIL_DeleteProbability, m_lDeleteMailProb);
	DDX_Text(pDX, IDC_EDIT_MAIL_SeverSub, m_lSeverSub);
	DDX_Text(pDX, IDC_EDIT_MAIL_SendMsgRepeatNum, m_lSendMsgRepeatNum);

	DDX_Control(pDX, IDC_COMBO_MAILSever, m_comboxServer);
	DDX_Control(pDX, IDC_CHECK_MAIL_SEND, m_CheckMailSend);		
	DDX_Control(pDX, IDC_CHECK_MAIL_RepeatSendName, m_CheckMailRepratName);
	DDX_Control(pDX, IDC_CHECK_MAILREC, m_CheckMailRec);
	DDX_Control(pDX, IDC_CHECK_MAIL_Reject, m_CheckMailReject);
	DDX_Control(pDX, IDC_CHECK_MAIL_Delete, m_CheckMailDelete);
	DDX_Control(pDX, IDC_CHECK_MAIL_GetGoods, m_CheckMailGetDoods);
	DDX_Control(pDX, IDC_CHECK_MAIL_AgreedPay, m_CheckMailAgreePay);
	DDX_Control(pDX, IDC_CHECK_MAIL_GetMoney, m_CheckMailGetMoney);
	DDX_Control(pDX, IDC_CHECK_MAIL_ExitClient, m_CheckMailExitClient);
	DDX_Control(pDX, IDC_CHECK_MAIL_RepeatSendMsg, m_CheckMailSendMsgRepeat);
	DDX_Control(pDX, IDC_CHECK_MAIL_PT_LOGIN, m_CheckMailPtLogin);

}


BEGIN_MESSAGE_MAP(CMailDlg, CDialog)

ON_BN_CLICKED(IDC_CHECK_MAIL_SEND, &CMailDlg::OnBnClickedCheckMailSend)
ON_BN_CLICKED(IDC_CHECK_MAIL_RepeatSendName, &CMailDlg::OnBnClickedCheckMailRepeatsendname)
ON_BN_CLICKED(IDC_CHECK_MAILREC, &CMailDlg::OnBnClickedCheckMailrec)
ON_BN_CLICKED(IDC_CHECK_MAIL_Reject, &CMailDlg::OnBnClickedCheckMailReject)
ON_BN_CLICKED(IDC_CHECK_MAIL_Delete, &CMailDlg::OnBnClickedCheckMailDelete)
ON_BN_CLICKED(IDC_CHECK_MAIL_GetGoods, &CMailDlg::OnBnClickedCheckMailGetgoods)
ON_BN_CLICKED(IDC_CHECK_MAIL_AgreedPay, &CMailDlg::OnBnClickedCheckMailAgreedpay)
ON_BN_CLICKED(IDC_CHECK_MAIL_GetMoney, &CMailDlg::OnBnClickedCheckMailGetmoney)
ON_BN_CLICKED(IDC_CHECK_MAIL_ExitClient, &CMailDlg::OnBnClickedCheckMailExitclient)
ON_BN_CLICKED(IDC_BUTTON_MailStart, &CMailDlg::OnBnClickedButtonMailstart)
ON_BN_CLICKED(IDC_CHECK_MAIL_RepeatSendMsg, &CMailDlg::OnBnClickedCheckMailRepeatsendmsg)
ON_BN_CLICKED(IDC_CHECK_MAIL_PT_LOGIN, &CMailDlg::OnBnClickedCheckMailLogin)
END_MESSAGE_MAP()


BOOL CMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_CheckMailSend.SetCheck(1);				//Ĭ���ܷ���
	//m_CheckMailRec.SetCheck(1);				//Ĭ��������
	//m_CheckMailRepratName.SetCheck(0);		//Ĭ�ϲ����ظ�����һ������
	//m_CheckMailReject.SetCheck(0);			//Ĭ�ϲ�������
	//m_CheckMailDelete.SetCheck(1);
	//m_CheckMailGetDoods.SetCheck(1);
	//m_CheckMailAgreePay.SetCheck(1);
	//m_CheckMailGetMoney.SetCheck(1);
	//m_CheckMailExitClient.SetCheck(0);
	m_comboxServer.InsertString(-1,"21������");
	m_comboxServer.InsertString(-1,"�Ÿ�40");
	m_comboxServer.InsertString(-1,"�Ÿ�50");
	m_comboxServer.InsertString(-1,"�Ƚ�");
	m_comboxServer.InsertString(-1,"����");
	m_comboxServer.InsertString(-1,"����");
	m_comboxServer.InsertString(-1,"�Ÿ�2");
	m_comboxServer.InsertString(-1,"26������");
	m_comboxServer.InsertString(-1,"���˷�");
	m_comboxServer.InsertString(-1,"ФҢ");
	m_comboxServer.InsertString(-1,"����");
	m_comboxServer.SetCurSel(0);
	return true;
}


void CMailDlg::OnBnClickedButtonMailstart()
{	
	UpdateData(TRUE);
	
	char strScriptName[256] = " " ;

	//SiMo��½��ʽ
	if ( !m_isCanPtLogin )	
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
		
		sprintf(strScriptName,"MailLoginCDKey(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s')",
												m_lStartCdKey,				//��ʼ�ʺ�
												m_lEndCdKey,				//�����ʺ�
												3,							//��������
												m_lMsgSendTimer,			//��Ϣ���
												m_lStartRegionID,			// ��ʼ����Id									
												m_lPosX,					// ����X									
												m_lPosY,					// ����Y									
												m_lSendMoney,				// ��Ǯ											
												m_lMailType,				// �ʼ����ͣ� -1Ϊ�������������											
												m_lGoodsNum,				// ��Ʒ����																															
												m_lSendRepeatNum,			// �ظ����ʹ���											
												m_lRejectMailProb,			// ���ż���											
												m_lExitClientPro,			// �����߼���										
												m_lDeleteMailProb,			// ɾ�ż���					
												m_lSeverSub,			    // �������汾
												m_lSendMsgRepeatNum,		//���ظ���һ���Ŵ���
												m_isCanSendMail,			//�Ƿ��ܷ���
												m_isCanRepeatSendName,		//�Ƿ��ظ����͸�һ�����
												m_isCanRecMail,				//�Ƿ�������
												m_isCanRejectMail,			//�Ƿ�����
												m_isCanDeleteMail,			//�Ƿ�ɾ��
												m_isCanGetGoods,			//�Ƿ�ȡ��Ʒ
												m_isCanAgreePay,			//�Ƿ�ͬ�⸶��
												m_isCanGetMoney,			//�Ƿ�ȡ��Ǯ
												m_isCanExitClient,			//�Ƿ�������
												m_isCanSendMsgRepeat,       //�Ƿ��ظ�����
												m_cstrSendName,				// ���͵Ľ���������
												m_cstrGoodsName		// ��Ʒ��
												);
	}
	else
	{
		if ( m_lStartCdKey < 0 )
		{	
			::MessageBox(NULL, "�������ʼ�ʺ�����С��0,����������", "ERRO!", MB_OK);
			return ;
		}

		//��SiMo��ʽ��һ��EndCDkey
		sprintf(strScriptName,"MailPtLoginCDKey(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s')",
			m_lStartCdKey,				//��ʼ�ʺ�
			//m_lEndCdKey,				//�����ʺ�
			3,							//��������
			m_lMsgSendTimer,			//��Ϣ���
			m_lStartRegionID,			// ��ʼ����Id									
			m_lPosX,					// ����X									
			m_lPosY,					// ����Y									
			m_lSendMoney,				// ��Ǯ											
			m_lMailType,				// �ʼ����ͣ� -1Ϊ�������������											
			m_lGoodsNum,				// ��Ʒ����																															
			m_lSendRepeatNum,			// �ظ����ʹ���											
			m_lRejectMailProb,			// ���ż���											
			m_lExitClientPro,			// �����߼���										
			m_lDeleteMailProb,			// ɾ�ż���					
			m_lSeverSub,			    // �������汾
			m_lSendMsgRepeatNum,		//���ظ���һ���Ŵ���
			m_isCanSendMail,			//�Ƿ��ܷ���
			m_isCanRepeatSendName,		//�Ƿ��ظ����͸�һ�����
			m_isCanRecMail,				//�Ƿ�������
			m_isCanRejectMail,			//�Ƿ�����
			m_isCanDeleteMail,			//�Ƿ�ɾ��
			m_isCanGetGoods,			//�Ƿ�ȡ��Ʒ
			m_isCanAgreePay,			//�Ƿ�ͬ�⸶��
			m_isCanGetMoney,			//�Ƿ�ȡ��Ǯ
			m_isCanExitClient,			//�Ƿ�������
			m_isCanSendMsgRepeat,       //�Ƿ��ظ�����
			m_cstrSendName,				// ���͵Ľ���������
			m_cstrGoodsName				// ��Ʒ��
			);
	}
	
	CScriptSys::getInstance()->PerformString(strScriptName);
}

//����
void CMailDlg::OnBnClickedCheckMailSend()
{
	m_isCanSendMail = m_CheckMailSend.GetCheck() ? true : false ;
}

//����	
void CMailDlg::OnBnClickedCheckMailrec()
{
	m_isCanRecMail = m_CheckMailRec.GetCheck() ? true : false ;
}

//�ظ���������
void CMailDlg::OnBnClickedCheckMailRepeatsendname()
{
	m_isCanRepeatSendName = m_CheckMailRepratName.GetCheck() ? true : false ;
}

//����
void CMailDlg::OnBnClickedCheckMailReject()
{
	m_isCanRejectMail = m_CheckMailReject.GetCheck() ? true : false ;
}

//ɾ��
void CMailDlg::OnBnClickedCheckMailDelete()
{
	m_isCanDeleteMail = m_CheckMailDelete.GetCheck() ? true : false ;
}

//ȡ��Ʒ
void CMailDlg::OnBnClickedCheckMailGetgoods()
{
	m_isCanGetGoods = m_CheckMailGetDoods.GetCheck() ? true : false ;
}

//ͬ�⸶��
void CMailDlg::OnBnClickedCheckMailAgreedpay()
{
	m_isCanAgreePay = m_CheckMailAgreePay.GetCheck() ? true : false ;
}

//ȡǮ
void CMailDlg::OnBnClickedCheckMailGetmoney()
{
	m_isCanGetMoney = m_CheckMailGetMoney.GetCheck() ? true : false ;
}

//������
void CMailDlg::OnBnClickedCheckMailExitclient()
{
	m_isCanExitClient = m_CheckMailExitClient.GetCheck() ? true : false ;
}

//�ظ�����һ����Ϣ
void CMailDlg::OnBnClickedCheckMailRepeatsendmsg()
{
	m_isCanSendMsgRepeat = m_CheckMailSendMsgRepeat.GetCheck() ? true : false ;
}

//��½��ʽ
void CMailDlg::OnBnClickedCheckMailLogin()
{
	m_isCanPtLogin =  m_CheckMailPtLogin.GetCheck() ? true : false ;
	if ( m_isCanPtLogin )
	{
		((CButton*)GetDlgItem(IDC_STATIC_strEndCDkey))->ShowWindow(false);	
		((CButton*)GetDlgItem(IDC_EDIT_MAIL_EndCdkey))->ShowWindow(false);
		((CButton*)GetDlgItem(IDC_STATIC_strStartCDkey))->SetWindowText("����������:");
	}
	else
	{
		((CButton*)GetDlgItem(IDC_STATIC_strEndCDkey))->ShowWindow(true);	
		((CButton*)GetDlgItem(IDC_EDIT_MAIL_EndCdkey))->ShowWindow(true);
		((CButton*)GetDlgItem(IDC_STATIC_strStartCDkey))->SetWindowText("��ʼ�˺�: GM");
	}
}
