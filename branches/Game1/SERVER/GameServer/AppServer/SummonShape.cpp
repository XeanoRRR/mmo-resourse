#include "stdafx.h"
#include <mmsystem.h>
#include "SummonShape.h"
#include "ServerRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//LONG CSummonShape::g_lID = 1;

CSummonShape::CSummonShape( tagMasterInfo& tMasterInfo, DWORD dwLifeTime )
{
	m_tSkillID		= SKILL_UNKNOW;
	m_tMasterInfo	= tMasterInfo;
	m_bMoveable		= TRUE;
	m_dwLifeTime	= dwLifeTime;
	m_dwTimeStamp	= timeGetTime();
	m_lType			= TYPE_SUMMON;
	//m_lID			= g_lID++;
}

CSummonShape::~CSummonShape()
{
}

VOID CSummonShape::SetAction( WORD act )
{
	m_wAction = act;
}

VOID CSummonShape::ForceMove( LONG lDestX, LONG lDestY, DWORD dwTime )
{
	//##������֤X,Y�Ƿ����ڳ����ķ�Χ��
	LONG lVerifiedDestX = lDestX;
	LONG lVerifiedDestY = lDestY;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		if( lDestX >= pRegion -> GetWidth() )
		{
			lVerifiedDestX = pRegion -> GetWidth() - 1;
		}
		if( lDestX < 0 )
		{
			lVerifiedDestX = 0;
		}
		if( lDestY >= pRegion -> GetHeight() )
		{
			lVerifiedDestY = pRegion -> GetWidth() - 1;
		}
		if( lDestY < 0 )
		{
			lVerifiedDestY = 0;
		}
	}
	else
	{
		return;
	}

	CMessage msg( MSG_S2C_SHAPE_FORCEMOVE );
	msg.Add( GetExID() );
	msg.Add( GetType() );
	msg.Add( GetTileX() );
	msg.Add( GetTileY() );
	msg.Add( lVerifiedDestX );
	msg.Add( lVerifiedDestY );
	msg.Add( dwTime );
	//##�ƶ�ʱ�Ķ���,�ͻ����Լ�����
	msg.Add( static_cast<DWORD>(0) );
	msg.SendToAround( this );

	//##�Ƶ��յ�
	SetPosXY( lVerifiedDestX+0.5f, lVerifiedDestY+0.5f );
}

VOID CSummonShape::MoveStep( LONG lDir )
{
	if( !m_bMoveable ) return;

	//##��ȡĿ���
	LONG lX = GetTileX(), lY = GetTileY();
	LONG lOldX = lX, lOldY = lY;
	GetDirPos( lDir, lX, lY );

	//##��֤����
	if( Distance( lX, lY ) > 1 ) return;

	//##�ƶ�û���赲�����ƣ�������赲���ǵ����赲����ô��ֹͣ��������ƶ�
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		if( pRegion -> GetBlock( lX, lY ) == CRegion::BLOCK_UNFLY )
		{
			return;
		}
		//##�ƶ���������Ϣ
		LONG lLastX, lLastY;
		lLastX = GetTileX();
		lLastY = GetTileY();

		SetPosXY( lX+0.5f, lY+0.5f );
		SetDir( lDir );
	}
}

VOID CSummonShape::End()
{
	CMessage msg( MSG_S2C_RGN_DELSHAPE );
	msg.Add( GetType() );
	msg.Add( GetExID() );
	msg.Add( static_cast<LONG>(0) );
	msg.SendToAround( this );
}

DWORD CSummonShape::GetRemainedTime()
{
	if( timeGetTime() >= m_dwTimeStamp + m_dwLifeTime )
		return 0;
	else return m_dwTimeStamp + m_dwLifeTime - timeGetTime();
}


//##�ٻ����������죬��������
CScope::CScope( DWORD dwLength, DWORD dwHeight )
{
	m_dwLength	= dwLength;
	m_dwHeight	= dwHeight;
	m_pScope	= new BYTE[dwLength * dwHeight];
	if( m_pScope )
	{
		ZeroMemory( m_pScope, dwLength * dwHeight );
	}
}

CScope::~CScope()
{
	SAFE_DELETE( m_pScope );
	m_dwLength	= 0;
	m_dwHeight	= 0;
}

BOOL CScope::Get( DWORD dwX, DWORD dwY )
{
	if( m_pScope == NULL ) return FALSE;
	if( dwX >= m_dwLength || dwY >= m_dwHeight ) return FALSE;
	if( m_pScope[dwY * m_dwLength + dwX] ) return TRUE;
	else return FALSE;
}

VOID CScope::Set( BYTE* pBfr )
{
	if( m_pScope == NULL || pBfr == NULL ) return;
	CopyMemory( m_pScope, pBfr, m_dwLength * m_dwHeight );
}

VOID CScope::Set( BYTE b, DWORD dwX, DWORD dwY )
{
	if( m_pScope == NULL ) return;
	if( dwX >= m_dwLength || dwY >= m_dwHeight ) return;
	m_pScope[dwY * m_dwLength + dwX] = b;
}

CScope& CScope::operator =( CScope& cIn )
{
	SAFE_DELETE( m_pScope );
	this -> m_dwLength	= cIn.m_dwLength;
	this -> m_dwHeight	= cIn.m_dwHeight;
	this -> Set( cIn.m_pScope );
	return( *this );
}

BOOL CScope::operator ==( CScope& cIn )
{
	if( this -> m_dwLength != cIn.m_dwLength ||
		this -> m_dwHeight != cIn.m_dwHeight ||
		this -> m_pScope == NULL ||	cIn.m_pScope == NULL ||
		memcmp( this -> m_pScope, cIn.m_pScope, 
		this -> m_dwLength * this -> m_dwHeight ) != 0 )
		return FALSE;
	return TRUE;
}

BOOL CScope::GetOverlappingField( tagRetangle& rSrc1, tagRetangle rSrc2, tagRetangle& rOut )
{
	//##��ȷ�����Ͻ�X���ص����
	//##Src2�����Ͻ�X��Src1��
	if( rSrc2.dwX >= rSrc1.dwX && rSrc2.dwX < rSrc1.dwX + rSrc1.dwLength )
	{
		rOut.dwX = rSrc2.dwX;
		//##���Ͻǵ�X���꣬Src2�����Ͻ���Src1��������
		if( rSrc2.dwX + rSrc2.dwLength > rSrc1.dwX + rSrc1.dwLength )
		{
			rOut.dwLength = rSrc1.dwX + rSrc1.dwLength - rOut.dwX;
		}
		//##Src2�����Ͻ���Src1��������
		else 
		{
			rOut.dwLength = rSrc2.dwX + rSrc2.dwLength - rOut.dwX;
		}
	}
	//##Src1�����Ͻ�X��Src2��
	else if( rSrc1.dwX >= rSrc2.dwX && rSrc1.dwX < rSrc2.dwX + rSrc2.dwLength )
	{
		rOut.dwX = rSrc1.dwX;
		//##���Ͻǵ�X���꣬Src1�����Ͻ���Src2��������
		if( rSrc1.dwX + rSrc1.dwLength > rSrc2.dwX + rSrc2.dwLength )
		{
			rOut.dwLength = rSrc2.dwX + rSrc2.dwLength - rOut.dwX;
		}
		//##Src1�����Ͻ���Src2��������
		else 
		{
			rOut.dwLength = rSrc1.dwX + rSrc1.dwLength - rOut.dwX;
		}
	}
	//##���ص�
	else return FALSE;

	//##��ȷ�����Ͻ�Y���ص����
	//##Src2�����Ͻ�Y��Src1��
	if( rSrc2.dwY >= rSrc1.dwY && rSrc2.dwY < rSrc1.dwY + rSrc1.dwHeight )
	{
		rOut.dwY = rSrc2.dwY;
		//##���Ͻǵ�Y���꣬Src2�����Ͻ���Src1��������
		if( rSrc2.dwY + rSrc2.dwHeight > rSrc1.dwY + rSrc1.dwHeight )
		{
			rOut.dwHeight = rSrc1.dwY + rSrc1.dwHeight - rOut.dwY;
		}
		//##Src2�����Ͻ���Src1��������
		else 
		{
			rOut.dwHeight = rSrc2.dwY + rSrc2.dwHeight - rOut.dwY;
		}
	}
	//##Src1�����Ͻ�Y��Src2��
	else if( rSrc1.dwY >= rSrc2.dwY && rSrc1.dwY < rSrc2.dwY + rSrc2.dwHeight )
	{
		rOut.dwY = rSrc1.dwY;
		//##���Ͻǵ�X���꣬Src1�����Ͻ���Src2��������
		if( rSrc1.dwY + rSrc1.dwLength > rSrc2.dwY + rSrc2.dwLength )
		{
			rOut.dwHeight = rSrc2.dwY + rSrc2.dwHeight - rOut.dwY;
		}
		//##Src1�����Ͻ���Src2��������
		else 
		{
			rOut.dwHeight = rSrc1.dwY + rSrc1.dwHeight - rOut.dwY;
		}
	}
	//##���ص�
	else return FALSE;

	return TRUE;
}