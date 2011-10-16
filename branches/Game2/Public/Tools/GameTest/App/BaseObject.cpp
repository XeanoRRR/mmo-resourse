// E:\Metempsychosis\App\BaseObject.cpp : implementation file
//

#include "stdafx.h"
#include "BaseObject.h"
#include "goods.h"
#include "Monster.h"
#include "player.h"
#include "region.h"
#include "../Script/ScriptSys.h"
#include "../GameTest/GameTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBaseObject

CBaseObject::CBaseObject()
:m_lType(0),
m_bIsHide(true),
m_bIncludeChild(true),
m_pFather(NULL)
{
	ZeroMemory(m_strName,32);
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
	/*m_lGraphicsID = */_GetLongFromByteArray(pByte, pos);

	_GetStringFromByteArray(pByte, pos, m_strName);

	return true;
}
bool CBaseObject::DecordFromDataBlock(DBReadSet& db_r,bool bEx /* = true */)
{
	m_bIsHide = (db_r.GetByteFromByteArray() != 0);
	m_lType = db_r.GetLongFromByteArray();
	db_r.GetBufferFromByteArray(m_guExID);
	/*m_lGraphicsID = */db_r.GetLongFromByteArray();
	db_r.GetStringFromByteArray(m_strName,256);
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

CBaseObject* CBaseObject::FindChildObject(const CGUID & guid)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CBaseObject* pObject = (*it);
		if (pObject && pObject->GetExID() == guid)
		{
			return pObject;
		}
	}
	return NULL;
}
//������object!
CBaseObject* CBaseObject::FindChildObject(long typeID,const CGUID& gID)
{
	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CBaseObject* pObject = (*it);
		if (pObject->IsThisObject(typeID,gID))
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
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
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
CBaseObject* CBaseObject::RecursiveFindObject(long typeID,CGUID& gID)
{
	if(IsThisObject(typeID, gID))
	{
		return this;
	}

	itBaseObject it;
	for (it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CBaseObject* pObject = (*it);
		CBaseObject* p = pObject->RecursiveFindObject(typeID,gID);
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
	if( typeID == m_lType && strcmp(strName, m_strName) == 0 )
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

// ɾ����OBJECT
void CBaseObject::DeleteChildObject(CBaseObject* pObj)
{
	RemoveObject(pObj);
	if(pObj)
	{
		SAFE_DELETE(pObj);
	}
}

// ɾ����OBJECT
void CBaseObject::DeleteChildObject(long typeID,const CGUID& gID)
{
	CBaseObject* pObj = FindChildObject(typeID,gID);
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
				CMoveShape* pShape = dynamic_cast<CMoveShape*>(pObject);
				if( pShape )
				{
					//if( pShape->GetUsedTimes() == 1 || pShape->GetUsedTimes() == 0 )
// 					if( pShape->GetUsedTimes() == 0 )
// 					{
						SAFE_DELETE(pObject);
/*					}*/
				}
				m_listObject.erase(it++);				
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

#if _DEBUG
	// ɾ��playerlist�м�¼
	if( pObject->GetType() == TYPE_PLAYER )
	{
		char str[256];
		sprintf(str, "%s �뿪���� %s(%d,%d)\n", pObject->GetName(), GetName(), 
			((CPlayer*)pObject)->GetTileX(), ((CPlayer*)pObject)->GetTileY());
		OutputDebugString(str);
	}
#endif
}

// ���OBJECT
void CBaseObject::AddObject(CBaseObject* pObject)
{
	pObject->SetFather(this);
	m_listObject.push_back(pObject);

#if _DEBUG
	// ����playerlist�б�
	if( pObject->GetType() == TYPE_PLAYER )
	{
		char str[256];
		sprintf(str, "%s ���볡�� %s(%d,%d)\n", pObject->GetName(),	GetName(),
			((CPlayer*)pObject)->GetTileX(), ((CPlayer*)pObject)->GetTileY());
		OutputDebugString(str);
	}
#endif
}

// �ص�һ�����������в��ҵ��Ķ�ӦTYPE����������
void CBaseObject::DgFindObjectsByTypes( long lType, P_FUNCTION_DO p_function_do, LPARAM lparam)
{
	listBaseObject listBack = m_listObject;
	for( itBaseObject it = listBack.begin();it!=listBack.end();it++)
	{
		CBaseObject* pObject = (*it);
		if( pObject->GetType() == lType )
		{
			p_function_do((LPARAM)pObject,lparam);
		}
		else{
			pObject->DgFindObjectsByTypes(lType,p_function_do,lparam);
		}
	}
}

Timer *QueryTimer()
{
    CGameTestDlg *dlg = (CGameTestDlg*) theApp.GetMainWnd();
    return dlg->GetTimer();
}
