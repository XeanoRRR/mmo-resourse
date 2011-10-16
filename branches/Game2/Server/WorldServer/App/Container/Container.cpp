// Copyright (C) 1991 - 1999 Rational Software Corporation
#include "StdAfx.h"
#include "Container.h"
#include "../BaseObject.h"
#include "../Listener/ContainerListener.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//--------------------------------------------------------------------------------------
// ����:	CContainer���캯��
// ����:	��
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A17B98003E
CContainer::CContainer()// : CBaseObject( TYPE_CONTAINER, 0 )
{
	RemoveAllListener();
}

//--------------------------------------------------------------------------------------
// ����:	CContainer��������
// ����:	��
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A17F83000F
CContainer::~CContainer()
{
	RemoveAllListener();
	Release();
}

//--------------------------------------------------------------------------------------
// ����:	CContainer���캯��
// ����:	��� : LONG
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A1B5160290
//CContainer::CContainer( LONG lID ) : CBaseObject( TYPE_CONTAINER, lID )
//{
//	RemoveAllListener();
//}

//--------------------------------------------------------------------------------------
// ����:	CalculateHashValue���������ֵ�ķ�����
// ����:	���� : OBJECT_TYPE����� : LONG
// ����ֵ:	DWORD
// ������	���ؼ���Ĺ���ֵ
//--------------------------------------------------------------------------------------
//##ModelId=41A1CD27037A
//DWORD CContainer::CalculateHashValue( /*OBJECT_TYPE*/LONG oType, LONG lID )
//{
//	DWORD dwHashValue = 0;
//	if( oType == TYPE_GOODS )
//	{
//		dwHashValue = lID;
//	}
//	else
//	{
//		dwHashValue = static_cast<BYTE>( oType );
//		dwHashValue	= dwHashValue << 24;
//		dwHashValue	= dwHashValue | ( lID & 0x00FFFFFF );
//	}
//	return dwHashValue;
//}

//--------------------------------------------------------------------------------------
// ����:	���ݹ���ֵ���������ڶ���ķ�����
// ����:	�ö���Ĺ���ֵ : DWORD
// ����ֵ:	CBaseObject*
// ������	�����ҵ��Ķ������ΪNULL���ʾû���ҵ���
//--------------------------------------------------------------------------------------
//##ModelId=41A3256C0242
CBaseObject* CContainer::Find(const CGUID& guid)
{
	// TODO: Add your specialized code here.
	return NULL;
}

//--------------------------------------------------------------------------------------
// ����:	��������ı��
// ����:	��� : LONG
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A32B6F008C
//VOID CContainer::SetID( LONG lID )
//{
//	CBaseObject::SetID( lID );
//}

//--------------------------------------------------------------------------------------
// ����:	��������ĸ�����
// ����:	���������� : CContainer*
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A32B5E038A
//VOID CContainer::SetFather( const CContainer* pFather )
//{
//	CBaseObject::SetFather( pFather );
//}

//--------------------------------------------------------------------------------------
// ����:	���ݹ���ֵ�Ƴ�������ĳ����ķ�����
// ����:	�ö���Ĺ���ֵ : DWORD
// ����ֵ:	CBaseObject*
// ������	�����Ƴ��˵Ķ���ָ�룬���ΪNULL���ʾ������û���ҵ��ö���
//--------------------------------------------------------------------------------------
//##ModelId=41A3E8020148
CBaseObject* CContainer::Remove( const CGUID& guid, LPVOID pVoid )
{
	// TODO: Add your specialized code here.
	return NULL;
}

//--------------------------------------------------------------------------------------
// ����:	AI
// ����:	��
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A3ED5B0290
//VOID CContainer::AI()
//{
//	// TODO: Add your specialized code here.
//	// NOTE: Requires a correct return value to compile.
//}

//--------------------------------------------------------------------------------------
// ����:	���ݶ���ָ����������ڶ���ķ�����
// ����:	�ö���ָ�� : CBaseObject*
// ����ֵ:	CBaseObject*
// ������	�����ҵ��Ķ������ΪNULL���ʾû���ҵ���
//--------------------------------------------------------------------------------------
//##ModelId=41A1913202FD
CBaseObject* CContainer::Find( CBaseObject* pObj )
{
	if( pObj != NULL )
	{
		//return Find( CContainer::CalculateHashValue( pObj->GetType(), pObj->GetID() ) );
		return Find( pObj->GetExID() );
	}
	return NULL;
}

//--------------------------------------------------------------------------------------
// ����:	�������ͺͱ�Ų��������ڶ���ķ�����
// ����:	���� : OBJECT_TYPE����� : LONG
// ����ֵ:	CBaseObject*
// ������	�����ҵ��Ķ������ΪNULL���ʾû���ҵ���
//--------------------------------------------------------------------------------------
//##ModelId=41A17F7601D4
CBaseObject* CContainer::Find( /*OBJECT_TYPE*/LONG oType, const CGUID& guid )
{
	return Find( guid );
}

//--------------------------------------------------------------------------------------
// ����:	���һ���������ڵķ�����
// ����:	�ö���ָ�� : CBaseObject*
// ����ֵ:	BOOL
// ������	����TRUE��ʾ��Ӷ���ɹ������ΪFALSE���ʾʧ�ܡ�
//--------------------------------------------------------------------------------------
//##ModelId=41A17D3B0186
BOOL CContainer::Add( CBaseObject* pObject, LPVOID pVoid )
{
	return FALSE;
}

//--------------------------------------------------------------------------------------
// ����:	������������ж��󣨵���ɾ����Щ���󣩵ķ�����
// ����:	��
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A17EA20242
VOID CContainer::Clear( LPVOID pVoid )
{
}

//--------------------------------------------------------------------------------------
// ����:	������������ж��󣨲���ɾ����Щ���󣩵ķ�����
// ����:	��
// ����ֵ:	��
// ������	��
//--------------------------------------------------------------------------------------
//##ModelId=41A191F100AB
VOID CContainer::Release()
{
	RemoveAllListener();
}

//--------------------------------------------------------------------------------------
// ����:	�Ƴ�������ĳһ���󣨵���ɾ����Щ���󣩵ķ�����
// ����:	�ö����ָ�� : CBaseObject*
// ����ֵ:	CBaseObject*
// ������	�����Ƴ��Ķ������ΪNULL���ʾ�����ڲ����ڸö���
//--------------------------------------------------------------------------------------
//##ModelId=41A17D5702EE
CBaseObject* CContainer::Remove( CBaseObject* pObject, LPVOID pVoid )
{
	if( pObject != NULL )
	{
		return Remove( pObject->GetExID(), pVoid );
	}
	return NULL;
}

//--------------------------------------------------------------------------------------
// ����:	�Ƴ�������ĳһ���󣨵���ɾ����Щ���󣩵ķ�����
// ����:	���� : OBJECT_TYPE����� : LONG
// ����ֵ:	CBaseObject*
// ������	�����Ƴ��Ķ������ΪNULL���ʾ�����ڲ����ڸö���
//--------------------------------------------------------------------------------------
//##ModelId=41A17DFC0213
CBaseObject* CContainer::Remove( /*OBJECT_TYPE*/LONG oType, const CGUID& guid, LPVOID pVoid )
{
	return Remove( guid, pVoid );
}

//--------------------------------------------------------------------------------------
// ����:	���һ������������
// ����:	pListener:����������
// ����ֵ:	TRUE or FALSE
// ������	
//--------------------------------------------------------------------------------------
//##ModelId=41A434660119
BOOL CContainer::AddListener(CContainerListener* pListener)
{
	if( pListener == NULL ) return FALSE;
	vector<CContainerListener*>::iterator it = m_vListeners.begin();
	for ( ; it != m_vListeners.end() ; it++ )
	{
		if( (*it) == pListener )
		{
			return FALSE;
		}
	}
	m_vListeners.push_back( pListener );
	return TRUE;
}

//--------------------------------------------------------------------------------------
// ����:	ɾ��һ������������
// ����:	pListener:����������
// ����ֵ:	TRUE or FALSE
// ������	
//--------------------------------------------------------------------------------------
//##ModelId=41A4348A02DE
BOOL CContainer::RemoveListener(CContainerListener* pListener)
{
	if( pListener == NULL ) return FALSE;
	vector<CContainerListener*>::iterator it = m_vListeners.begin();
	for ( ; it != m_vListeners.end() ; it++ )
	{
		if( (*it) == pListener )
		{
			m_vListeners.erase( it );
			return TRUE;
		}
	}
	return FALSE;
}

//--------------------------------------------------------------------------------------
// ����:	ɾ����������������
// ����:	
// ����ֵ:	
// ������	
//--------------------------------------------------------------------------------------
//##ModelId=41A4349E00CB
VOID CContainer::RemoveAllListener()
{
	m_vListeners.clear();
}

