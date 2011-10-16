#pragma once
#include "../../../public/GUID.h"


//ҵ��״̬
enum eBusinessState
{
	eBSTATE_PRELOCK=1, //�ʻ�Ԥ��ǰ
	eBSTATE_LOCKED,	   //�ʻ���Ԥ��
	eBSTATE_WAITFOR_ITEM,	//�ȴ����ŵ��߷���
	eBSTATE_ITEM_SENDED,	//���߷��ͷ���
	eBSTATE_ACCOUNT_UPDATED	//�ʻ�����
};

class CSession 
{
private:	
	CGUID m_guid;

	string m_strCdkey;	

	BOOL m_bSessionEnded;

	BOOL m_bSessionStarted;

	BOOL m_bSessionAborted;	

	DWORD m_dwStartingTimeStamp;

	DWORD m_dwLifeTime;	

	BOOL m_bRemoveFlag;

	eBusinessState m_eBusinessState;
public:	
	const CGUID&	GetExID()	const			{return m_guid;};
	void			SetExID(const CGUID& guid)	{m_guid = guid;};
	BOOL IsSessionEnded();	

	BOOL IsSessionAvailable();

	BOOL IsSessionShouldBeenRemoved();				
	
	CSession(const char* strCdkey);
	
	~CSession();
	
	VOID AI();	
	
	BOOL Start( BOOL bFlag = FALSE );
	
	BOOL End( BOOL bFlag = FALSE );
	
	BOOL Abort( BOOL bFlag = FALSE );

	BOOL OnSessionStarted( BOOL bFlag );

	BOOL OnSessionEnded( BOOL bFlag );

	BOOL OnSessionAborted( BOOL bFlag );

	VOID SetBusinessState(eBusinessState eState){m_eBusinessState=eState;}

	eBusinessState GetBusinessState(){return m_eBusinessState;}
};