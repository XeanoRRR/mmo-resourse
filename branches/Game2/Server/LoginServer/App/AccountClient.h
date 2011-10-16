/**
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-15
*/
#pragma once

#include "LoaclAccountValidate.h"



class AccountClient
{
public:
	//!	ValidateAccount()�ӿڵķ���ֵ
	enum eValidateReturn
	{
		//! �ȴ�AS��֤
		eVR_WaitASValidate,
		//! ������֤ͨ��
		eVR_LocalPass,
		//! ������֤ʧ��
		eVR_LocalLose,
	};

public:
	AccountClient(VOID);
	~AccountClient(VOID);

public:
	//!						��ʼ��
	BOOL					Init();

	//!						����AS�׽���
	VOID					SetASSocket(LONG lASSocket)		{m_lASSocket = lASSocket;};
	//!						�õ�AS�׽���
	LONG					GetASSocket(VOID)				{return m_lASSocket;};
	
	//!						��֤һ���˺�
	BOOL					ValidateAccount(tagAccountInfo_ToAS &AccountInfo);

	//!						�ڱ��ر���һ��ͨ����֤���˺�
	VOID					SaveAccount(tagAccountInfo_FromAS &AccountInfo_FromAS, const tagSecurityData *pSecurityData);

	//!						���
	VOID					RejectAccount(const char *pCdKey, LONG lEndTime);

	//!						������֤
	BOOL					LocalValidate(tagAccountInfo_ToAS &AccountInfo);


private:
	
	//!								AS�������׽���
	LONG							m_lASSocket;

	//!								�������ݻ���
	LoaclAccountValidate			*m_pLoaclAccountValidate;

};