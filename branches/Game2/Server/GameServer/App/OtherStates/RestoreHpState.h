#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_RESTORE_HP_STATE_INCLUDED
#define _INC_RESTORE_HP_STATE_INCLUDED
#include "..\States\State.h"
#include "..\States\VisualEffect.h"

class CRestoreHpState : public CState
{
private:
	//##����ʱ���Ǻ���Ϊ��λ
	DWORD									m_dwTimeToKeep;
	DWORD									m_dwFrequency;
	//##��Ѫ���
	DWORD									m_dwHPGain;
	//##��Ѫ����
	DWORD									m_dwRestoreHpCounts;

public:
	CRestoreHpState							( DWORD dwPersistTime, DWORD dwFrequency, DWORD dwHPGain );
	CRestoreHpState							();
	virtual	~CRestoreHpState				();
	
	virtual BOOL	Begin					( CMoveShape* pUser, CMoveShape* pSufferer );
	virtual BOOL	Begin					( CMoveShape* pUser, LONG lX, LONG lY );
	virtual BOOL	Begin					( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion );

	virtual VOID	AI						();
	virtual VOID	End						();

	virtual VOID	OnChangeRegion			( LONG );
	virtual DWORD	GetRemainedTime			();

	//##���л�
	virtual VOID	Serialize				( vector<BYTE>& vOut );
	//##�����л�
	virtual VOID	Unserialize				( BYTE* pBytes, LONG& lPos );
};


class CRestoreHpStateVisualEffect : public CVisualEffect
{
public:
	virtual VOID		UpdateVisualEffect	( CState* pParent, DWORD dwCmd );
};

#endif