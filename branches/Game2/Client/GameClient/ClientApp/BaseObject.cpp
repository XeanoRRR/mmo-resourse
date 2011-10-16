// E:\Metempsychosis\App\BaseObject.cpp : implementation file
//

#include "stdafx.h"
#include "BaseObject.h"
#include "../../../Public/Common/Public.h"
#include "../../../Public/Common/BaseDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBaseObject

CBaseObject::CBaseObject()
{
	m_bIsHide = false;
	m_lType = 0;
	m_lGraphicsID = 0;
	m_strName = "";
	m_pFather = NULL;

	m_bIncludeChild = true;
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
	return true;
}

// ����
bool CBaseObject::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	m_bIsHide = (_GetByteFromByteArray(pByte, pos) != 0);
	m_lType = _GetLongFromByteArray(pByte, pos);
	_GetBufferFromByteArray(pByte,pos,m_guExID);
	m_lGraphicsID = _GetLongFromByteArray(pByte, pos);

	char str[256];
	m_strName = _GetStringFromByteArray(pByte, pos, str);

	return true;
}

// ����
bool CBaseObject::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	m_bIsHide = (setReadDB.GetByteFromByteArray() != 0);
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
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
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
CBaseObject* CBaseObject::FindChildObject(long typeID,const CGUID& id)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CBaseObject* pObject = (*it);
		if (pObject->IsThisObject(typeID,id))
		{
			return pObject;
		}
	}
	return NULL;
}


CBaseObject* CBaseObject::FindChildObject(const CGUID& id)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CBaseObject* pObject = (*it);
		if (pObject && pObject->GetExID() == id)
		{
			return pObject;
		}
	}
	return NULL;
}

// ������goods
CBaseObject* CBaseObject::FindChildGoods(const CGUID& guExID)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CBaseObject* pObject = (*it);
		if (pObject->GetType() == TYPE_GOODS && pObject->GetExID() == guExID)
		{
			return pObject;
		}
	}
	return NULL;
}

// �ݹ����
CBaseObject* CBaseObject::RecursiveFindObject(long typeID,CGUID& id)
{
	if(IsThisObject(typeID, id))
	{
		return this;
	}

	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CBaseObject* pObject = (*it);
		CBaseObject* p = pObject->RecursiveFindObject(typeID,id);
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
	if( typeID == m_lType && strcmp(strName, GetOrigName()) == 0 )
	{
		return this;
	}

	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
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
void CBaseObject::DeleteChildObject(long typeID,const CGUID& ID)
{
	CBaseObject* pObj = FindChildObject(typeID,ID);
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
		for (itBaseObject it=m_listObject.begin(); it!=m_listObject.end();)
		{
			CBaseObject* pObject = (*it);
			if( pExpect != pObject )
			{
				m_listObject.erase(it++);
				SAFE_DELETE(pObject);
			}
			else
			{
				++it;
			}
		}
	}
}

// �Ƴ�OBJECT
void CBaseObject::RemoveObject(CBaseObject* pObject)
{
	m_listObject.remove(pObject);

}

// ���OBJECT
void CBaseObject::AddObject(CBaseObject* pObject)
{
	pObject->SetFather(this);
	m_listObject.push_back(pObject);

}

// �ص�һ�����������в��ҵ��Ķ�ӦTYPE����������
void CBaseObject::DgFindObjectsByTypes( long lType, P_FUNCTION_DO p_function_do, LPARAM lparam)
{
	listBaseObject listBack = m_listObject;
	for (itBaseObject it = listBack.begin(); it != listBack.end(); ++it)
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
		itBaseObject it;
		for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
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
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		(*it)->BoardCast(lCommand, lParam);
	}
}
