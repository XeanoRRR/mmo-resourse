#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_TEAM_STATE_INCLUDED
#define _INC_TEAM_STATE_INCLUDED

#include "..\States\State.h"
#include "..\States\VisualEffect.h"

class CTeamState : public CState
{
	string							m_strTeamName;
	string							m_strTeamPassword;
	DWORD							m_dwLastCheckTimeStamp;

public:
	CTeamState						( CHAR* pName, CHAR* pPassword );
	CTeamState						();
	virtual ~CTeamState				();
	
	virtual BOOL	Begin			( CMoveShape* pUser, CMoveShape* pSufferer );
	virtual BOOL	Begin			( CMoveShape* pUser, LONG lX, LONG lY );
	virtual BOOL	Begin			( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion );

	virtual VOID	AI				();
	virtual VOID	End				();

	virtual VOID	OnChangeRegion	( LONG );

	virtual DWORD	GetAdditionalData	();

	CHAR*			GetTeamName		();
	CHAR*			GetTeamPassword	();

	//##�������ʩ����ʱ�������ĳ�ֻ,������·,�ܲ�,����,��������
	//##״̬ȱʡ�Ĳ����Ǽ������������״̬�ڱ�������ʱ��ᱻ�ж�
	//__inline virtual VOID OnAction( tagAction ta ) 
	//{
	//	if( ta == ACTION_DEFENSE )
	//	{
	//		End();
	//	}
	//}

	//##���л�
	virtual VOID Serialize( vector<BYTE>& vOut );

	//##�����л�
	virtual VOID Unserialize( BYTE* pBytes, LONG& lPos );
};


class CTeamStateVisualEffect : public CVisualEffect
{
public:
	virtual VOID		UpdateVisualEffect	( CState* pParent, DWORD dwCmd );
};

#endif