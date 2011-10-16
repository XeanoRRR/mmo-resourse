#pragma once
#include "shape.h"

class CMoveShape :
	public CShape
{
public:
	CMoveShape(void);
	virtual ~CMoveShape(void);

//------------------------------------------------------------------------
// �쳣״̬
//------------------------------------------------------------------------
protected:
	vector<BYTE>						m_vExStates;

public:
	VOID			SetExStates			( BYTE* pBytes, LONG lSize );
	vector<BYTE>&	GetExStates			()	{return m_vExStates;}

//////////////////////////////////////////////////////////////////////////
//	�޵�ģʽ
//////////////////////////////////////////////////////////////////////////
public:

	void God( bool bIsGod = true )		{ m_bIsGod = bIsGod; }
	bool IsGod()						{ return m_bIsGod; }

protected:
	bool m_bIsGod;
};
