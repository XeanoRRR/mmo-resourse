#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _UseGoodsEnlargeDefState_h_
#define _UseGoodsEnlargeDefState_h_
#include "..\States\State.h"
#include "..\States\VisualEffect.h"

class CUseGoodsEnlargeDefState : public CState
{
	//##����ʱ���Ǻ���Ϊ��λ
	DWORD							m_dwTimeToKeep;
	DWORD							m_dwCoefficient;

public:
	CUseGoodsEnlargeDefState				( DWORD dwTimeToKeep, DWORD dwCoefficient );
	CUseGoodsEnlargeDefState				();
	virtual ~CUseGoodsEnlargeDefState		();

	virtual BOOL OnUpdateProperties ();

	virtual BOOL	Begin			( CMoveShape* pUser, CMoveShape* pSufferer );
	virtual BOOL	Begin			( CMoveShape* pUser, LONG lX, LONG lY );
	virtual BOOL	Begin			( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion );

	virtual VOID	AI				();
	virtual VOID	End				();

	//##��Ҫ�޸�ʹ���ߵ�RegionID
	virtual VOID	OnChangeRegion	( LONG );
	//##���״̬û��ʱ������
	virtual DWORD	GetRemainedTime ();

	//##���л�
	virtual VOID	Serialize		( vector<BYTE>& vOut );
	//##�����л�
	virtual VOID	Unserialize		( BYTE* pBytes, LONG& lPos );
};

class CUseGoodsEnlargeDefVisualEffect : public CVisualEffect
{
public:
	virtual VOID		UpdateVisualEffect	( CState* pParent, DWORD dwCmd );
};

#endif