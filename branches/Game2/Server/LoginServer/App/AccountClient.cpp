#include "stdafx.h"
#include "AccountClient.h"

#include "../Net/Message.h"



AccountClient::AccountClient(VOID)
:m_lASSocket(0)
,m_pLoaclAccountValidate(NULL)
{}
AccountClient::~AccountClient(VOID)
{
	//! ����û��ִ�е��������
	LoaclAccountValidate::ReleasValidateOperator(&m_pLoaclAccountValidate);
	m_pLoaclAccountValidate = NULL;
}


//! ��ʼ��
BOOL AccountClient::Init()
{
	return FALSE;
}

//! ��֤һ���˺�
BOOL AccountClient::ValidateAccount(tagAccountInfo_ToAS &AccountInfo)
{
	if(0 != GetASSocket())
	{
		//! AS��֤
		CMessage msg(MSG_O2A_ACCOUNT_Validate_Request);
		msg.AddEx(&AccountInfo, sizeof(tagAccountInfo_ToAS));
		msg.SendToAS();
		return TRUE;
	}

	//! ������֤
	return LocalValidate(AccountInfo);
}

//! �ڱ��ر���һ��ͨ����֤���˺�
VOID AccountClient::SaveAccount(tagAccountInfo_FromAS &AccountInfo_FromAS, const tagSecurityData *pSecurityData)
{
	if(NULL == m_pLoaclAccountValidate)
		m_pLoaclAccountValidate = LoaclAccountValidate::CreateSaveOperator();

	//! ������ʧ��
	if(!m_pLoaclAccountValidate->AddSaveItem(AccountInfo_FromAS, pSecurityData))
	{
		//! ˵������������ִ��
		LoaclAccountValidate *pLoaclAccountValidate = m_pLoaclAccountValidate;
		m_pLoaclAccountValidate = NULL;
		if(!pLoaclAccountValidate->SaveResult())
			LoaclAccountValidate::ReleasValidateOperator(&pLoaclAccountValidate);

		//! ���·���һ���ɻ���Ķ���
		m_pLoaclAccountValidate = LoaclAccountValidate::CreateSaveOperator();
		//! �������
		if(NULL != m_pLoaclAccountValidate)
			m_pLoaclAccountValidate->AddSaveItem(AccountInfo_FromAS, pSecurityData);
	}
}

//! ���
VOID AccountClient::RejectAccount(const char *pCdKey, LONG lEndTime)
{
	if(NULL == pCdKey) return;
	if(0 == strlen(pCdKey)) return;

	CMessage msg(MSG_O2A_ACCOUNT_Reject_Request);
	msg.Add(pCdKey);
	msg.Add(lEndTime);
	msg.SendToAS();
}

//! ������֤
BOOL AccountClient::LocalValidate(tagAccountInfo_ToAS &AccountInfo)
{
	//assert(0 == GetASSocket());
	AccountInfo._bLocalValidate = true;
	LoaclAccountValidate *pLoaclAccountValidate = LoaclAccountValidate::CreateValidateOperator(AccountInfo);

	assert(NULL != pLoaclAccountValidate);
	if(NULL == pLoaclAccountValidate)
	{
		CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
		msg.Add((LONG)LOGIN_PASSWORDERROR);
		msg.SendToClient(AccountInfo._lSocket);
		return FALSE;
	}
	if(!pLoaclAccountValidate->BeginValidate())
	{
		//! ����
		CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
		msg.Add((LONG)LOGIN_PASSWORDERROR);
		msg.SendToClient(AccountInfo._lSocket);
		LoaclAccountValidate::ReleasValidateOperator(&pLoaclAccountValidate);
		return FALSE;
	}

	return TRUE;
}