
/*
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-15
*/

#include "..\..\public\AccountServerDef.h"
#include "LoaclAccountValidate.h"

#pragma once


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

#ifdef ENABLE_LOCAL_VALIDATE
	//!						������֤
	BOOL					LocalValidate(tagAccountInfo_ToAS &AccountInfo);
#endif


private:
	
	//!								AS�������׽���
	LONG							m_lASSocket;

#ifdef ENABLE_LOCAL_VALIDATE
	//!								�������ݻ���
	LoaclAccountValidate			*m_pLoaclAccountValidate;
#endif

};