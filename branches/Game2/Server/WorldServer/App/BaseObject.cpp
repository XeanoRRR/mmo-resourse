// E:\Metempsychosis\App\BaseObject.cpp : implementation file
//

#include "StdAfx.h"
#include "BaseObject.h"
#include "Goods/Goods.h"
#include "Goods/GoodsFactory.h"
#include "Monster.h"
#include "Player.h"
#include "Region.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef __MEMORY_LEAK_CHECK__
	long CBaseObject::m_nConstructFlag=0;
	CBaseObject::mapLLL	CBaseObject::m_mapConstructObjects;
#endif
// CBaseObject

CBaseObject::CBaseObject()
{
	m_lType = 0;
	//m_lID = 0;
	m_lGraphicsID = 0;
	m_strName = "";
	m_pFather = NULL;

	m_bIncludeChild = true;

	m_byCanHide = 0; // �ܷ�����
	m_byHideFlag = 0; // ��ǰ����״̬
}

CBaseObject::~CBaseObject()
{
	DeleteAllChildObject();
}

// CBaseObject member functions

/////////////////////////////////////////////////////////////////////
// ��ȡ�ӿ�
/////////////////////////////////////////////////////////////////////

// ��ȡ
BOOL CBaseObject::Load()
{
	return true;
}

// �洢
BOOL CBaseObject::Save()
{
	return true;
}

// ��ӵ�CByteArray
bool CBaseObject::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	_AddToByteArray(pByteArray, GetType());
	_AddToByteArray(pByteArray, GetExID());
	_AddToByteArray(pByteArray, GetGraphicsID());
	_AddToByteArray(pByteArray, (char*)GetName());
	return true;
}

// ����
bool CBaseObject::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	m_lType = _GetLongFromByteArray(pByte, pos);
	_GetBufferFromByteArray(pByte, pos, m_guExID);
	m_lGraphicsID = _GetLongFromByteArray(pByte, pos);

	char str[256];
	m_strName =_GetStringFromByteArray(pByte, pos, str);

	return true;
}

// ��ӵ�CByteArray
bool CBaseObject::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData)
{
	// �������书��
	setWriteDB.AddToByteArray((BYTE)m_byCanHide);
	setWriteDB.AddToByteArray((BYTE)m_byHideFlag);

	setWriteDB.AddToByteArray(GetType());
	setWriteDB.AddToByteArray(GetExID());
	setWriteDB.AddToByteArray(GetGraphicsID());
	setWriteDB.AddToByteArray((char*)GetName());
	return true;
}

// ����
bool CBaseObject::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	m_byCanHide = setReadDB.GetByteFromByteArray();
	m_byHideFlag = setReadDB.GetByteFromByteArray();

	m_lType = setReadDB.GetLongFromByteArray();
	setReadDB.GetBufferFromByteArray(m_guExID);
	m_lGraphicsID =setReadDB.GetLongFromByteArray();
	char str[256];
	m_strName =setReadDB.GetStringFromByteArray(str,256);
	return true;
}

/////////////////////////////////////////////////////////////////////
// �ӽڵ��б�������������
/////////////////////////////////////////////////////////////////////

//������object!
bool CBaseObject::FindChildObject(CBaseObject* p)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CBaseObject* pObject = (*it);
		if (pObject == p)
		{
			return true;
		}
	}
	return false;
}

//������object!
//CBaseObject* CBaseObject::FindChildObject(long typeID,long lID)
CBaseObject* CBaseObject::FindChildObject(long typeID, const CGUID& guExID)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CBaseObject* pObject = (*it);
		if (pObject->IsThisObject(typeID,guExID))
		{
			return pObject;
		}
	}
	return NULL;
}

// �ݹ����
CBaseObject* CBaseObject::RecursiveFindObject(long typeID, const CGUID& guExID)
{
	if(IsThisObject(typeID, guExID))
	{
		return this;
	}

	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CBaseObject* pObject = (*it);
		CBaseObject* p = pObject->RecursiveFindObject(typeID,guExID);
		if (p != NULL)
		{
			return p;
		}
	}
	return NULL;
}

// �����ֲ���
CBaseObject* CBaseObject::RecursiveFindObject(long typeID,char* strName)
{
	if( typeID == m_lType && strcmp(strName, m_strName.c_str()) == 0 )
	{
		return this;
	}

	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CBaseObject* pObject = (*it);
		CBaseObject* p = pObject->RecursiveFindObject(typeID, strName);
		if (p != NULL)
		{
			return p;
		}
	}
	return NULL;
}

// ����Object
CBaseObject* CBaseObject::CreateObject(long typeID, const CGUID& guid)
{
	int *nNum = 0;
	CBaseObject* pObject = NULL;

	switch(typeID)
	{
	case TYPE_GOODS:		// ��Ʒ
		{
#ifdef __MEMORY_LEAK_CHECK__
			CBaseObject::SetConstructFlag(2);
#endif
			pObject = new CGoods;
			CGoodsFactory::AddCreateGoodsLog((CGoods*)pObject,27);
		}
		break;

	case TYPE_MONSTER:		// ����
		{
			pObject = new CMonster;
		}
		break;

	//case TYPE_NPC:			// NPC
	//	{
	//		pObject = new CNpc;
	//	}
	//	break;

	case TYPE_PLAYER:		// ���
		{
			pObject = new CPlayer;
		}
		break;

	case TYPE_REGION:		// ��ͼ
		{
			pObject = new CRegion;
		}
		break;

	default:
		return NULL;
	}
	
	pObject->SetExID(guid);
	pObject->SetType(typeID);
	return pObject;
}

// ������OBJECT
/*CBaseObject* CBaseObject::CreateChildObject(long typeID, const CGUID& guExID, const char* name, long lGraphicsID)
{
	CBaseObject* pObj = CreateObject(typeID,guExID);
	if( !pObj )
	{
		return NULL;
	}

	// �½�obj
	//if( lID == 0 )
	//{
	//	CRsSetup* pSetup = theApp.GetDataBase()->m_pRsSetup;
	//	int i=0;
	//	//pSetup->_Open();
	//	if( pSetup->_Edit() )
	//	{
	//		switch(typeID)
	//		{
	//		case TYPE_GOODS:		// ��Ʒ
	//			{
	//				lID = pSetup->GetNewGoodsId();
	//			}
	//			break;

	//		case TYPE_PLAYER:		// ���
	//			{
	//				lID = pSetup->GetNewPlayerID();
	//			}
	//			break;
	//		}

	//		//pSetup->RefreshSeedNum();
	//	}
	//	else
	//	{
	//		ASSERT(0);
	//		delete pObj;
	//		return NULL;
	//	}
	//}

	if( name != NULL )
	{
		pObj->m_strName = name;
	}
	CGUID lBackID = pObj->GetExID();
	pObj->SetExID(guExID);

	// ��������
	if( typeID == TYPE_PLAYER && lBackID == NULL_GUID  || 
		typeID == TYPE_GOODS && lBackID == NULL_GUID && strcmp(name, "NoAdd") == 0 )
	{
	}
	else
	{
		AddObject(pObj);
	}

	pObj->SetExID(lBackID);
	pObj->Load();
	pObj->SetExID( guExID );

	if( lGraphicsID != 0 )
	{
		pObj->SetGraphicsID( lGraphicsID );
	}

	if( name != NULL )
	{
		pObj->m_strName = name;
	}

	return pObj;
}*/

// ɾ����OBJECT
void CBaseObject::DeleteChildObject(CBaseObject* pObj)
{
	RemoveObject(pObj);
	if(pObj){
		delete pObj;
		pObj = NULL;
	}
}

// ɾ����OBJECT
//void CBaseObject::DeleteChildObject(long typeID,long lID)
void CBaseObject::DeleteChildObject(long typeID, CGUID& guExID)
{
	CBaseObject* pObj = FindChildObject(typeID,guExID);
	if (pObj == NULL)
	{
		return ;
	}

	DeleteChildObject(pObj);
}

// ɾ��������OBJECT(pExpect = ����)
void CBaseObject::DeleteAllChildObject(CBaseObject* pExpect)
{
	if( m_listObject.size() )
	{
		listBaseObject list = m_listObject;
		for (itBaseObject it = list.begin(); it != list.end(); it++)
		{
			CBaseObject* pObject = (*it);
			if( pExpect != pObject )
			{
				DeleteChildObject(pObject);
			}
		}
		list.clear();
	}
}

// �Ƴ�OBJECT
void CBaseObject::RemoveObject(CBaseObject* pObject)
{
	m_listObject.remove(pObject);

	// ɾ��PlayerList�м�¼
	if( pObject->GetType() == TYPE_PLAYER )
	{
	//	theApp.GetPlayerList()->remove((CPlayer*)pObject);

#if _DEBUG
		char str[256];
		sprintf(str, "%s �뿪���� %s(%f,%f)\n", pObject->GetName(), GetFather()->GetName(), 
			((CPlayer*)pObject)->GetPosX(), ((CPlayer*)pObject)->GetPosY());
		OutputDebugString(str);
#endif
	}
}

// ���OBJECT
void CBaseObject::AddObject(CBaseObject* pObject)
{
	pObject->SetFather(this);
	m_listObject.push_back(pObject);

	// ����PlayerList�б�
	if( pObject->GetType() == TYPE_PLAYER )
	{
	//	theApp.GetPlayerList()->push_back((CPlayer*)pObject);

#if _DEBUG
		char str[256];
		sprintf(str, "%s ���볡�� %s(%f,%f)\n", pObject->GetName(), GetFather()->GetName(),
			((CPlayer*)pObject)->GetPosX(), ((CPlayer*)pObject)->GetPosY());
		OutputDebugString(str);
#endif
	}
}

// �ص�һ�����������в��ҵ��Ķ�ӦTYPE����������
void CBaseObject::DgFindObjectsByTypes( long lType, P_FUNCTION_DO p_function_do, LPARAM lparam)
{
	listBaseObject listBack = m_listObject;
	for (itBaseObject it = listBack.begin(); it != listBack.end(); it++)
	{
		CBaseObject* pObject = (*it);
		if( pObject->GetType() == lType )
		{
			p_function_do((LPARAM)pObject, lparam);
		}
		else
		{
			pObject->DgFindObjectsByTypes(lType, p_function_do, lparam);
		}
	}
}

/////////////////////////////////////////////////////////////////////
// AI
/////////////////////////////////////////////////////////////////////

void CBaseObject::AI()		// AI����
{
	if( m_listObject.size() )
	{
		listBaseObject listBack = m_listObject;
		itBaseObject it;
		for (it = listBack.begin(); it != listBack.end(); it++)
		{
			CBaseObject* pObject = (*it);
			pObject->AI();
		}
	}
}

/////////////////////////////////////////////////////////////////////
// �㲥
/////////////////////////////////////////////////////////////////////

// �㲥
void CBaseObject::BoardCast(long lCommand, long lParam)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		(*it)->BoardCast(lCommand, lParam);
	}
}


#ifdef __MEMORY_LEAK_CHECK__

void CBaseObject::AddConstructRef(long lType)
{
	itLLL itObj = m_mapConstructObjects.find(lType);
	if( itObj != m_mapConstructObjects.end() )
	{
		mapLL &ObjNum = (*itObj).second;
		itLL itNum = ObjNum.find(m_nConstructFlag);
		if(itNum != ObjNum.end())
		{
			(*itNum).second++;
		}
		else
		{
			ObjNum[m_nConstructFlag]=1;
		}
	}
	else
	{
		m_mapConstructObjects[lType][m_nConstructFlag]=1;
	}
}

void CBaseObject::RefConstructRef(long lType,long lConFlag)
{
	itLLL itObj = m_mapConstructObjects.find(lType);
	if( itObj != m_mapConstructObjects.end() )
	{
		mapLL &ObjNum = (*itObj).second;
		itLL itNum = ObjNum.find(lConFlag);
		if(itNum != ObjNum.end())
		{
			(*itNum).second--;
		}
	}
}

void CBaseObject::OutputConstructRef(long lType)
{
	char pszStrInfo[256]="";
	itLLL itObj = m_mapConstructObjects.begin();
	for( ;itObj != m_mapConstructObjects.end();itObj++ )
	{
		mapLL &ObjNum = (*itObj).second;
		itLL itNum = ObjNum.begin();
		for(;itNum != ObjNum.end();itNum++)
		{
// 			_snprintf(pszStrInfo,256,"objType:%d,flag:%d,Num:%d",(*itObj).first,(*itNum).first,(*itNum).second);
// 			PutStringToFile("ObjReleaseCheck",pszStrInfo);

            Log4c::Trace(ROOT_MODULE,"objType:%d,flag:%d,Num:%d",(*itObj).first,(*itNum).first,(*itNum).second);
		}
	}
}

#endif
