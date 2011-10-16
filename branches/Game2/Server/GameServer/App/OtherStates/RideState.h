#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BOA_RIDE_STATE_INCLUDED
#define _INC_BOA_RIDE_STATE_INCLUDED

#include "..\States\State.h"
#include "..\States\VisualEffect.h"

class CRideState : public CState
{
	DWORD							m_dwHorseType;
	DWORD							m_dwHorseLevel;
	DWORD							m_dwRoleLimit;
	DWORD							m_dwCheckGoodsTimeStamp;

	string							m_strGoodName;			//����ʹ��������Ʒ������

public:
	CRideState						( DWORD dwType, DWORD dwLevel, DWORD dwRoleLimit );
	CRideState						();
	virtual ~CRideState				();

	void	SetGoodName(string strName)		{m_strGoodName = strName;}
	string	GetGoodName()					{return m_strGoodName;}
	
	virtual BOOL	OnUpdateProperties();
	virtual BOOL	Begin			( CMoveShape* pUser, CMoveShape* pSufferer );
	virtual BOOL	Begin			( CMoveShape* pUser, LONG lX, LONG lY );
	virtual BOOL	Begin			( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion );

	virtual VOID	AI				();
	virtual VOID	End				();

	virtual VOID	OnChangeRegion	( LONG );

	virtual DWORD	GetAdditionalData	();

	DWORD			GetHorseType	();
	DWORD			GetHorseLevel	();

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


class CRideStateVisualEffect : public CVisualEffect
{
public:
	virtual VOID		UpdateVisualEffect	( CState* pParent, DWORD dwCmd );
};

#endif