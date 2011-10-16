#include "StdAfx.h"
#include "NetSession.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNetSession::CNetSession(const CGUID& guid, pair<CGUID,long> CookiePair)
:m_guid(guid)
,m_Cookie(CookiePair)
,m_pCallback(NULL)
,m_dwTimeOut(0)
{
}

CNetSession::~CNetSession(void)
{
	if(m_pCallback)
	{
		m_pCallback->Release();//�ͷ����Լ�
	}
}


//��֤Cookie
bool	CNetSession::CheckCookie(const CGUID& PlayerGuid,long lPassWord)
{
	if(PlayerGuid == m_Cookie.first && lPassWord == m_Cookie.second)	return true;
	return false;
}

//���ûص��������
void CNetSession::SetCallbackHandle(IAsyncCallback* pCallbackHandle)
{
	assert(pCallbackHandle);
	m_pCallback = pCallbackHandle;
}

//��ʼ�첽����
void CNetSession::Beging(ulong dwTimeOut,IAsyncCaller* pCaller,...)
{
	assert(pCaller);
	m_dwTimeOut	= dwTimeOut;

	va_list va;
	va_start(va,pCaller);

	//ͬ�����ô���
	pCaller->DoAsyncCall(GetGUID(),m_Cookie.second,va);
	va_end(va);

}

//�ص�ʱ����
void	CNetSession::OnDo(va_list va)
{
	assert(m_pCallback);
	if(m_pCallback)
	{
		tagAsyncResult Result;
		Result.Result	 = AR_DO;
		Result.Arguments = va;
		//���ص�����
		m_pCallback->OnAsyncCallback(Result);
	}
}
//�ص�����ʱ����
void CNetSession::OnResult(va_list va)
{
	assert(m_pCallback);
	if(m_pCallback)
	{
		tagAsyncResult Result;
		Result.Result	 = AR_OK;
		Result.Arguments = va;
		//���ص�����
		m_pCallback->OnAsyncCallback(Result);
	}
}

//��س�ʱ
void CNetSession::OnTimeOut()
{
	assert(m_pCallback);
	if(m_pCallback)
	{
		tagAsyncResult Result;
		Result.Result	 = AR_TimeOut;
		//���ص�����
		m_pCallback->OnAsyncCallback(Result);
	}
}