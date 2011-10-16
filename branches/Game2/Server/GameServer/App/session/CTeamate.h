/*
// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CTEAMATE_INCLUDED
#define _INC_CTEAMATE_INCLUDED

#include "CPlug.h"

class CTeamate : public CPlug
{
public:
	enum TEAMATE_STATE
	{
		//##���忪ʼ
		TS_BEGIN				= CPlug::PS_USER_DEFINE_BEGIN,
		//##�ı�ӳ�����Ҫ�����������
		TS_CHANGE_LEADER,
		//##�ı���䷽ʽ����Ҫ�����������
		TS_CHANGE_ALLOCATION_SCHEME,
		//##�ı䳡������Ҫ�����������
		TS_CHANGE_REGION,
		//##�ı����ԣ�����HP��MP��RP��
		TS_PROPERTIES_UPDATED,
		//##����
		TS_CHAT,
		//##�ı�״̬
		TS_CHANGE_STATE,
	};

private:

	LONG	m_lOwnerRegionID;
	DWORD	m_dwPlayerLoseTimeStamp;
	string	m_strOwnerName;

public:

	CTeamate();

	~CTeamate();

	virtual BOOL IsPlugAvailable();

	virtual BOOL OnSessionEnded();

	virtual BOOL OnSessionAborted();

	virtual BOOL OnPlugInserted();

	virtual BOOL OnPlugAborted();

	virtual BOOL OnPlugEnded();

	virtual BOOL OnChangeState(LONG lPlugID, LONG lStateID, BYTE* pData);

	//virtual VOID Release();

	VOID	SetOwnerRegionID( LONG lID );
	LONG	GetOwnerRegionID();

	VOID	SetOwnerName( CHAR* szName );
	CHAR*	GetOwnerName();

	virtual BOOL Serialize(vector<BYTE>* pStream);

	virtual BOOL Unserialize(BYTE* pStream, LONG& lOffset);

};

#endif
*/