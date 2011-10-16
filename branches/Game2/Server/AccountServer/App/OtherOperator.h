
/*
/**
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-21
*/
#pragma once

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
	//!									����һ���û������߲���
	static		OtherOperator*			CreateLoginAndLogout(const char* pCdKey, LONG lLSID, eLoginAndLogoutType eLogin);

	//!									���ٲ���
	static		VOID					ReleaseOperator(OtherOperator **ppOperator);

	///////////////////////////////////////////////////////////
	//! �첽���ݿ����
public:
	//!									ִ�з�Ų���
	VOID								RejectAccount(VOID);
	//!									��¼�����ߣ���÷�����ʱ�䣨�룩
	VOID								LoginAndLogout(VOID);

	///////////////////////////////////////////////////////////
	//! ͬ�����ݿ����
public:
	//!									��DB�л��һ��LS�ϵ������û�
	static		BOOL					GetLsOnlineAccount(LONG lLSID, set<string> &setAccount);
	//!									ִ�а�ȫ��֤����
	static		BOOL					UserSecurityOperate(const char* pCdKey, DWORD dwIp, LONG &lWorkFlag, char arrSecurityCard[]);
	//!									�û������ߴ���
	static		LONG					UserLoginAndLogout(const char* pCdKey, LONG lLSID, DWORD dwLogin, set<DWORD> &setLSID);

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
	//!									�û�������
	static		LPVOID					LoginAndLogoutFromDB(LPVOID pThis);

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

private:
	static MultiMemPool<LockOfWin32>	c_MultiMemPool;



//! ��������Ϳ���
private:
	OtherOperator(void);
	OtherOperator(OtherOperator&);
	~OtherOperator(void);
};
