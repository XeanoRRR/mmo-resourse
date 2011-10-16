#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_AUTOMATIC_RESTORE_MP_STATE_INCLUDED
#define _INC_AUTOMATIC_RESTORE_MP_STATE_INCLUDED
#include "..\States\State.h"
#include "..\States\VisualEffect.h"

class CAutomaticRestoreMpState : public CState
{
private:
	//##Ƶ��
	DWORD									m_dwFrequency;
	//##��Ѫ�� 
	FLOAT									m_fVolume;


public:
	CAutomaticRestoreMpState				( DWORD dwFrequency, FLOAT fVolume );
	CAutomaticRestoreMpState				();
	virtual	~CAutomaticRestoreMpState		();
	
	virtual BOOL	Begin					( CMoveShape* pUser, CMoveShape* pSufferer );
	virtual BOOL	Begin					( CMoveShape* pUser, LONG lX, LONG lY );
	virtual BOOL	Begin					( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion );

	virtual VOID	AI						();
	virtual VOID	End						();

	virtual VOID	OnChangeRegion			( LONG );

	//##���л�
	virtual VOID	Serialize				( vector<BYTE>& vOut );
	//##�����л�
	virtual VOID	Unserialize				( BYTE* pBytes, LONG& lPos );
};


class CAutomaticRestoreMpStateVisualEffect : public CVisualEffect
{
public:
	virtual VOID		UpdateVisualEffect	( CState* pParent, DWORD dwCmd );
};

#endif