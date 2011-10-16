// E:\Metempsychosis\App\BaseObject.cpp : implementation file
//

#include "stdafx.h"
#include "BaseObject.h"
#include "goods\cgoods.h"
#include "goods\CGoodsFactory.h"
#include "npc.h"
#include "Monster.h"
#include "player.h"
#include "region.h"
#include "../public/public.h"



#ifdef __MEMORY_LEAK_CHECK__
	long CBaseObject::m_nConstructFlag=0;
	CBaseObject::mapLLL	CBaseObject::m_mapConstructObjects;
#endif
// CBaseObject

CBaseObject::CBaseObject()
{
	m_lType = 0;
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
			pObject = MP_NEW CGoods;
			CGoodsFactory::AddCreateGoodsLog((CGoods*)pObject,27);
		}
		break;

	case TYPE_MONSTER:		// ����
		{
			pObject = MP_NEW CMonster;
		}
		break;
	case TYPE_PLAYER:		// ���
		{
			pObject = MP_NEW CPlayer;
		}
		break;

	case TYPE_REGION:		// ��ͼ
		{
			pObject = MP_NEW CRegion;
		}
		break;

	default:
		return NULL;
	}
	
	pObject->SetExID(guid);
	pObject->SetType(typeID);
	return pObject;
}


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

	// ɾ��playerlist�м�¼
	if( pObject->GetType() == TYPE_PLAYER )
	{
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

	// ����playerlist�б�
	if( pObject->GetType() == TYPE_PLAYER )
	{
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
			_snprintf(pszStrInfo,256,"objType:%d,flag:%d,Num:%d",(*itObj).first,(*itNum).first,(*itNum).second);
			PutStringToFile("ObjReleaseCheck",pszStrInfo);
		}
	}
}

#endif


tagTimerVar* CBaseObject::CreateTimerVar(long Flag)
{
	tagTimerVar *pRe = (tagTimerVar*)M_ALLOC(sizeof(tagTimerVar));
#ifdef _DEBUG_TIMER_VAR_
	new(pRe)tagTimerVar(Flag);
#else
	new(pRe)tagTimerVar();
#endif

	return pRe;
}

void CBaseObject::ReleaseTimerVar(tagTimerVar **ppTimer)
{
	if (NULL != ppTimer && NULL != *ppTimer)
	{
		tagTimerVar* pTimvar = *ppTimer;

		M_FREE(pTimvar, sizeof(tagTimerVar));
		*ppTimer = NULL;
	}
}
