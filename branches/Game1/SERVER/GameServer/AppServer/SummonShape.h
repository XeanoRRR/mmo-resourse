#pragma once
#include "Shape.h"
#include "..\..\..\Nets\MySocket.h"
#include "MasterInfo.h"
#include "..\..\..\Public\SkillRelated.h"

//##�ٻ���û���������ٻ���,���綾��,ħ����
class CSummonShape : public CShape
{
protected:
	//##���˵��й���Ϣ
	tagMasterInfo									m_tMasterInfo;
	//##�Ƿ�����ƶ�
	BOOL											m_bMoveable;
	//##�������ȣ�0��ʾ����ʧ
	DWORD											m_dwLifeTime;
	//##����ʱ��
	DWORD											m_dwTimeStamp;

	tagSkillID										m_tSkillID;

	//static LONG										g_lID;

	DWORD				GetRemainedTime				();

public:
	//##����/��������
    CSummonShape									( tagMasterInfo& tMasterInfo, DWORD dwLifeTime );
	~CSummonShape									();

	//##���õ�ǰ����
	virtual VOID		SetAction					( WORD act );
	//##���յ���Ϣ��Ĵ���
	virtual VOID		OnMessage					( CMessage* pMsg ) {}
	//##AI���
	virtual VOID		AI							() = 0;
    //##�ƶ�
	virtual VOID		MoveStep					( LONG lDir );
	//##ǿ���ƶ�
	virtual VOID		ForceMove					( LONG lDestX, LONG lDestY, DWORD dwTime );

	//##�����ٻ����ܵ�����
	virtual tagSkillID	GetSkillID					()
	{
		return m_tSkillID;
	}
	//##��ͬ���ܵ����������滻
	virtual VOID		ReplaceAffectRegion			( LONG lLevel, LONG lXStart, LONG lYStart ) = 0;

	virtual	VOID		End							();

	virtual bool		AddToByteArray				( vector<BYTE>* pByteArray, bool bExData = true ) = 0;
	
	virtual bool		DecordFromByteArray			( BYTE* pByte, LONG& lPos, bool bExData ) { return true; }
};

struct tagRetangle
{
	DWORD											dwX;
	DWORD											dwY;
	DWORD											dwLength;
	DWORD											dwHeight;
};

//##�ٻ������������Щ���������״
class CScope
{
	BYTE*											m_pScope;
	DWORD											m_dwLength;
	DWORD											m_dwHeight;

public:
	CScope											( DWORD dwLength, DWORD dwHeight );
	~CScope											();

	//##��ȡ�˸��Ƿ�Ϊ��������
	BOOL				Get							( DWORD dwX, DWORD dwY );
	//##���ú�������
	VOID				Set							( BYTE* pBfr );
	VOID				Set							( BYTE b, DWORD dwX, DWORD dwY );

	//##��ȡ��
	__inline DWORD		GetLength					() 
	{
		return m_dwLength;
	}
	//##��ȡ��
	__inline DWORD		GetHeight					()
	{
		return m_dwHeight;
	}

	CScope&				operator =					( CScope& cIn );
	BOOL				operator ==					( CScope& cIn );

	//##�ж�2�������Ƿ��ص�,����ص�,�򷵻��ص�����Ĵ�С
	static BOOL			GetOverlappingField			( tagRetangle& rSrc1, tagRetangle rSrc2, tagRetangle& rOut );
};