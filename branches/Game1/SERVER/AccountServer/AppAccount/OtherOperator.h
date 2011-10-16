
/*
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-21
*/

#include "ValidateOperator.h"

#pragma once

//! ִ�з����Բ�ѯ�ķ���
enum eLoginAndLogoutType
{
	eLALT_Logout	= 0,	//! ��½�˺�
	eLALT_Login		= 1,	//! �˳��˺�
	eLALT_GetTime	= 2,	//! �ޱ仯�˺ţ�ֻ�����¼���ʱ��
};


class OtherOperator
{
public:
	static		VOID					Release(VOID);
public:
	//!									����һ����Ų���
	static		OtherOperator*			CreateRejectOperator(const char* pCdKey, LONG lEndTime);

#ifdef ENABLE_FCM
	//!									����һ���û������߲���
	static		OtherOperator*			CreateLoginAndLogout(const char* pCdKey, LONG lLSID, eLoginAndLogoutType eLogin);
#endif
	//!									���ٲ���
	static		VOID					ReleaseOperator(OtherOperator **ppOperator);

	///////////////////////////////////////////////////////////
	//! �첽���ݿ����
public:
	//!									ִ�з�Ų���
	VOID								RejectAccount(VOID);
#ifdef ENABLE_FCM
	//!									��¼�����ߣ���÷�����ʱ�䣨�룩
	VOID								LoginAndLogout(VOID);
#endif

	///////////////////////////////////////////////////////////
	//! ͬ�����ݿ����
public:


	//!									ִ�а�ȫ��֤����
	static		BOOL					UserSecurityOperate(const char* pCdKey, DWORD dwIp, LONG &lWorkFlag, char arrSecurityCard[]);
#ifdef ENABLE_FCM
	//!									�û������ߴ���
	static		LONG					UserLoginAndLogout(const char* pCdKey, LONG lLSID, DWORD dwLogin, set<DWORD> &setLSID);
	//!									��DB�л��һ��LS�ϵ������û�
	static		BOOL					GetLsOnlineAccount(LONG lLSID, set<string> &setAccount);
#endif

	//! UserLoginAndLogout�����ķ���ֵ����
	enum eUserLoginAndLogoutRe
	{
		//! �洢���̶���ķ���ֵ
		eULALR_RejoinLs = -20,	//! ���󣬶�ε�½���˳�ͬһ��LS
		eULALR_NoLimit = -10,	//! û��ʱ������
		
		//! �����Զ����ֵ
		eULALR_RunLose = -9999,
	};

	
private:
	//!									��ŵ����ݲ���
	static		LPVOID					RejectAccountFromDB(LPVOID pThis);
#ifdef ENABLE_FCM
	//!									�û�������
	static		LPVOID					LoginAndLogoutFromDB(LPVOID pThis);
#endif

private:
	//! ��Ų���������
	struct tagRejectData
	{
		char		_szCdKey[CDKEY_SIZE];
		LONG		_lEndTime;
	};

	//! �����ߵ�����
	struct tagLoginAndlogout
	{
		char		_szCdKey[CDKEY_SIZE];
		LONG		_lLSID;
		DWORD		_dwSecond;
		DWORD		_dwLogin;
		set<DWORD>	_setLSID;
	};


	enum eDateType
	{
		eDT_NoData,
		eDT_RejectData,
		eDT_LoginAndlogout,
	};

private:
	//!									����ָ��
	LPVOID								m_pData;
	//!									��������
	DWORD								m_dwDataType;

//! ��������Ϳ���
private:
	OtherOperator(void);
	OtherOperator(OtherOperator&);
	~OtherOperator(void);
};